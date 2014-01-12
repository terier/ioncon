#include "CGLUTApplication.h"
#include <stdio.h>
#include <time.h>
#include <sstream>

#include "CObjectMesh.h"
#include "CObjectShapes.h"
#include "CObjectSkyDome.h"
#include "utils.h"
#include "physics.h"
#include "icr_loader.h"
#include "icc_loader.h"
#include "glsl.h"
#include "CShaderDefaultCallback.h"
#include "blockGenerator.h"
#include "roadGenerator.h"

CGLUTApplication::CGLUTApplication(const SGLUTParameters& param) :
	DoubleBuffering(param.DoubleBuffering)
{
	printf("Creating GLUT application ...\n");
	unsigned int mode = GLUT_DEPTH | GLUT_RGBA;
	if (param.DoubleBuffering)
		mode |= GLUT_DOUBLE;
	if (param.FSAA)
		mode |= GLUT_MULTISAMPLE;

	int argc = 1;
	char* argv = "glutprogram";
	glutInit(&argc, &argv);
	glutInitContextVersion(2, 0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(mode);
	glutInitWindowPosition(param.WindowX, param.WindowY);
	glutInitWindowSize(param.WindowW, param.WindowH);
	if (param.WindowTitle)
		glutCreateWindow(param.WindowTitle);
	else
		glutCreateWindow("");

	car = param.Car;
	skyDome = param.SkyDome;

	glewInit();
	init();
}

CGLUTApplication::~CGLUTApplication()
{
	delete Scene;
	delete Physics;
	delete Camera;
}

void CGLUTApplication::init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5f);

	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(100,100);

	for (int i=0; i<256; i++)
	{
		KeyDown[i] = false;
		KeyDownPrev[i] = false;
	}

	Scene = new CScene();
	Physics = new CPhysicsWorld();
	ShaderManager = new cwc::glShaderManager();
	CameraFPS = new CCameraFPS();
	CameraFPS->setPosition(vec3(15,10,30));
	CameraFPS->setSpeed(200.f);
	CameraFPS->setFar(6000.f);

	Camera = new CCameraFollower(0);
	Camera->setDistance(20.f);
	Camera->setDisplacement(vec3(0,12,0));
	Camera->setViewDisplacement(vec3(0,-2,0));
	Camera->setStiffness(50.f);
	Camera->setSpeed(0.4f);

	CameraBumper = new CCameraFollower(0);
	CameraBumper->setDistance(0);
	CameraBumper->setDisplacement(vec3(0,3,9.5f));
	CameraBumper->setViewDisplacement(vec3(0,0,10));
	CameraBumper->setStiffness(50.f);
	CameraBumper->setSpeed(3.f);

	float v[4] = {1,1,1,1};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, v);
	//glLightf

	Scene->setActiveCamera(CameraFPS);
	Scene->setClearColor(vec3(0.10f, 0.11f, 0.15f));

	// sky box test
	uint skytex = Scene->loadTexture(skyDome);
	CObjectSkyDome* sky = new CObjectSkyDome(skytex);
	Scene->setSkyDome(sky);

	// road
	CSpline* spline;
	uint roadtex = Scene->loadTexture("images/roadtex3.png");
	uint normaltex = Scene->loadTexture("images/normal2.jpg", true);
	uint stenciltex = Scene->loadTexture("images/roadstencil.jpg", true);
	//CMesh* roadMesh = loadRoad("models/testroad.icr", &spline);
	SRoadProperties roadprops;
	loadRoadProperties("models/testroad.icr", roadprops);
	roadprops.Subdiv = 1000;
	spline = generateRoad(13, 1530, 200, PI * 0.1f);
	//spline->makeCardinal(0.7f);
	Spline = spline;
	CMesh* roadMesh = new CMesh(spline, roadprops.Stencil, roadprops.Subdiv, roadprops.ScaleTexture, roadprops.ScaleStencil);
	CObjectMesh* roadObject = new CObjectMesh(roadMesh);
	roadObject->setTexture(0, roadtex);
	roadObject->setTexture(1, normaltex);
	roadObject->setTexture(2, stenciltex);
	Scene->addObjectToRoot(roadObject);
	CPhysicsObject* roadPhysics = Physics->addStaticMeshObject(roadObject);
	roadPhysics->getPhysicsObject()->setRestitution(0.2f);

	cwc::glShader* shader = ShaderManager->loadfromFile(
		"shaders/irr.vert",
		"shaders/irr.frag");
	CShaderCallback* callback = new CShaderDefaultCallback(shader);
	roadObject->setShader(shader);
	roadObject->setShaderCallback(callback);

	// put some lamps ...
	CMesh* lampMesh = new CMesh("models/lamp01.obj");
	float tend = (float) spline->getNumberOfControlPoints();
	float dt = tend / 100.f;
	mat4 basis;
	for (float t=0; t<tend; t+=dt)
	{
		spline->getFrameBasisFromStencil(t, basis, vec3(28, 7, 0));
		CObject* lamp = Scene->addObjectMesh(lampMesh);
		lamp->setTransformation(basis.getTransposed());
	}

	// add checkpoints
	CPController = new CObjectCheckpointController();
	Scene->addObjectToRoot(CPController); // for animation and querying
	int nnn = 10;
	for (int i=0; i<spline->getNumberOfControlPoints() * nnn; i++)
		CPController->addCheckpoint(SCheckpoint(spline->getPosition(i/(float)nnn), 30.f));

	// put a creepy red sphere above the starting position

	CObject* redSphere = new CObjectMesh(new CMesh("models/startflag.obj"));
	redSphere->setPosition(spline->getPosition(0) + vec3(0,15,0));
	redSphere->setTexture(0, Scene->loadTexture("images/startflag.png", true));
	Scene->addObjectToRoot(redSphere);

	// CARS -------------------------------------------------------------------

	float nControlPoints = (float)Spline->getNumberOfControlPoints();
	Vehicle = addCar(car);
	cwc::glShader* phongShader = ShaderManager->loadfromFile("shaders/phong.vert", "shaders/phong.frag");
	((CObjectMesh*)Vehicle->getRenderObject())->setShader(phongShader);
	Camera->setFollowedObject(Vehicle->getRenderObject());
	CameraBumper->setFollowedObject(Vehicle->getRenderObject());
	CPController->addObjectTracker(Vehicle->getRenderObject(), "Player");

	CCar* car2 = addCar("models/cars/corvette.icc");
	((CObjectMesh*)car2->getRenderObject())->setShader(phongShader);
	CPController->addObjectTracker(car2->getRenderObject(), "Corvette");

	CCar* car3 = addCar("models/cars/california.icc");
	((CObjectMesh*)car3->getRenderObject())->setShader(phongShader);
	CPController->addObjectTracker(car3->getRenderObject(), "California");

	CCar* car4 = addCar("models/cars/SL500.icc");
	((CObjectMesh*)car4->getRenderObject())->setShader(phongShader);
	CPController->addObjectTracker(car4->getRenderObject(), "SL500");

	Overlay = new COverlayText("", GLUT_BITMAP_HELVETICA_18, vec3(0,1,0), false);
	//overlay->setParent(Vehicle->getRenderObject());
	Scene->addObjectToRoot(Overlay);

	bool placeLeft = true; // altering left - right
	btTransform position;
	float startPoint = 0;
	for (size_t i=0; i<Vehicles.size(); i++)
	{
		mat4 trans;
		placeLeft = !placeLeft;
		if (placeLeft)
		{
			Spline->getFrameBasisFromStencil(startPoint, trans, vec3(10, 5, 0));
			startPoint -= 20.f / Spline->getDerivative(startPoint).len();
		}
		else
		{
			Spline->getFrameBasisFromStencil(startPoint, trans, vec3(-10, 5, 0));
		}
		trans = getOpenGLMatrixFromFrameBasis(trans);
		position.setFromOpenGLMatrix(trans.M);
		Vehicles[i]->getPhysicsObject()->setWorldTransform(position);
	}

	// ---------------------------------------------------------------------------------------------

	generateCity();

	isPlaying = true;
	step();
	for (size_t i=0; i<Vehicles.size(); i++)
		Vehicles[i]->updateWheelTransform();
	step();
	isPlaying = false;
}


void CGLUTApplication::generateCity()
{
	float radius = 1800;
	float angle = 180.f*DEGTORAD;
	float ang;
	cwc::glShader* phongShader = ShaderManager->loadfromFile("shaders/phong.vert", "shaders/phong.frag");
	
	// city
	CMesh* city = new CMesh("models/architecture/wild_town.obj");
	CObjectMesh* cityObject = Scene->addObjectMesh(city);
	cityObject->setPosition(vec3(0,-80,0));

	// block generator
	uint startTex = Scene->loadTexture("models/blockGenerator/blockStart2.png");
	uint itemTex = Scene->loadTexture("models/blockGenerator/blockItem2.png");
	blockGenerator* gener = new blockGenerator("models/blockGenerator/blockStart2.obj",
												"models/blockGenerator/blockItem2.obj",
												"models/blockGenerator/blockEnd2.obj",
												startTex, itemTex, 0);
	ang = frandm()*angle+PI;
	std::vector<CObjectMesh*> geneBlockObjVect = gener->generateBlock(5,9,3,vec3(cos(ang)*radius*1.1f,-30,sin(ang)*radius*1.1f));
	for(size_t i=0; i<geneBlockObjVect.size(); i++)
		Scene->addObjectToRoot(geneBlockObjVect[i]);
	ang = frandm()*angle+PI;
	geneBlockObjVect = gener->generateBlock(8,11,3,vec3(cos(ang)*radius*1.1f,-30,sin(ang)*radius*1.1f));
	for(size_t i=0; i<geneBlockObjVect.size(); i++)
		Scene->addObjectToRoot(geneBlockObjVect[i]);
	ang = frandm()*angle+PI;
	geneBlockObjVect = gener->generateBlock(5,15,3,vec3(cos(ang)*radius*1.1f,-30,sin(ang)*radius*1.1f));
	for(size_t i=0; i<geneBlockObjVect.size(); i++)
		Scene->addObjectToRoot(geneBlockObjVect[i]);
	ang = frandm()*angle+PI;
	geneBlockObjVect = gener->generateBlock(8,11,3,vec3(cos(ang)*radius*1.1f,-30,sin(ang)*radius*1.1f));
	for(size_t i=0; i<geneBlockObjVect.size(); i++)
		Scene->addObjectToRoot(geneBlockObjVect[i]);
	ang = frandm()*angle+PI;
	geneBlockObjVect = gener->generateBlock(8,18,5,vec3(cos(ang)*radius*1.1f,-30,sin(ang)*radius*1.1f));
	for(size_t i=0; i<geneBlockObjVect.size(); i++)
		Scene->addObjectToRoot(geneBlockObjVect[i]);
	ang = frandm()*angle+PI;
	geneBlockObjVect = gener->generateBlock(6,12,4,vec3(cos(ang)*radius*1.1f,-30,sin(ang)*radius*1.1f));
	for(size_t i=0; i<geneBlockObjVect.size(); i++)
		Scene->addObjectToRoot(geneBlockObjVect[i]);

	// skyscrapers
	CMesh* skyscraper = new CMesh("models/architecture/newYorkStyle.obj");
	CObjectMesh* tempSkyscraper;
	for(int i=0; i<10; i++) {
		ang = frandm()*angle+PI;
		tempSkyscraper = Scene->addObjectMesh(skyscraper);
		tempSkyscraper->setPosition(vec3(cos(ang)*radius*1.3f,-30,sin(ang)*radius*1.4f));
		tempSkyscraper->setRotation(vec3(0, frand() * 2 * PI, 0));
		tempSkyscraper->setShader(phongShader);
	}

	// business block
	CMesh* businessBlock = new CMesh("models/architecture/businessBlock.obj");
	CObjectMesh* tempBusinessBlock;
	for(int i=0; i<10; i++) {
		ang = frandm()*angle+PI;
		tempBusinessBlock = Scene->addObjectMesh(businessBlock);
		tempBusinessBlock->setPosition(vec3(cos(ang)*radius*1.1f,-30,sin(ang)*radius*1.2f));
		tempBusinessBlock->setRotation(vec3(0, -ang-PI2, 0));
		tempBusinessBlock->setShader(phongShader);
	}

	// big house
	CMesh* bigHouse = new CMesh("models/architecture/bigHouse.obj");
	CObjectMesh* tempBigHouse;
	for(int i=0; i<10; i++) {
		ang = frandm()*angle+PI;
		tempBigHouse = Scene->addObjectMesh(bigHouse);
		tempBigHouse->setPosition(vec3(cos(ang)*radius,-15,sin(ang)*radius));
		tempBigHouse->setRotation(vec3(0, -ang-PI2, 0));
		tempBigHouse->setShader(phongShader);
	}
}

void CGLUTApplication::step()
{
	float dt = getTimeStep();
	if (isPlaying)
		Physics->getWorld()->stepSimulation(dt, 10);
	Scene->animate(dt);

	if (KeyDown['1'])
		Scene->setActiveCamera(Camera);
	if (KeyDown['2'])
		Scene->setActiveCamera(CameraBumper);
	if (KeyDown['3'])
		Scene->setActiveCamera(CameraFPS);

	if (keyPressed('p')) {
		isPlaying = !isPlaying;
		if(isPlaying)
			Timer.start();
		else
			Timer.stop();
	}

	if (KeyDown['w'] || KeyDown['W'])
		CameraFPS->moveForward(dt);
	if (KeyDown['s'] || KeyDown['S'])
		CameraFPS->moveBackward(dt);
	if (KeyDown['a'] || KeyDown['A'])
		CameraFPS->moveLeft(dt);
	if (KeyDown['d'] || KeyDown['D'])
		CameraFPS->moveRight(dt);

	if (KeyDown['i'] || KeyDown['I'] || KeyDown['k'] || KeyDown['K'])
	{
		if (KeyDown['i'] || KeyDown['I'])
			Vehicle->accelerate();
		else
			Vehicle->reverse();
	}
	else
		Vehicle->idle();

	if (KeyDown['j'] || KeyDown['J'] || KeyDown['l'] || KeyDown['L'])
	{
		if (KeyDown['j'] || KeyDown['J'])
			Vehicle->steerLeft(dt);
		else
			Vehicle->steerRight(dt);
	}
	else
		Vehicle->steerStraight(dt);

	if (KeyDown[' '])
		Vehicle->brake();
	else
		Vehicle->brakeRelease();

	// checkpoint test
	static uint current = -1;
	static float bestlap = 0;
	static float laptime = 0;
	uint ln = CPController->getLapNum(Vehicle->getRenderObject());
	if (current != ln)
	{
		laptime = Timer.getTimeSeconds();
		Timer.reset();
		if (bestlap == 0 || laptime < bestlap)
			bestlap = laptime;
		current = ln;
	}

	// basic AI
	for (size_t i=0; i<Vehicles.size(); i++)
	{
		if (Vehicles[i] == Vehicle)
			continue;
		//VehiclesClosest[i]->setPosition(Spline->getPosition(Spline->getClosestPoint(Vehicles[i]->getRenderObject()->getPosition())));
		float t = Spline->getClosestPoint(Vehicles[i]->getRenderObject()->getPosition());
		float optimalSpeed = 1.f / (0.001f + 0.2f * Spline->getCurvature(t));
		float currentSpeed = abs(Vehicles[i]->getVehicle()->getCurrentSpeedKmHour());
		if (currentSpeed < optimalSpeed)
		{
			Vehicles[i]->accelerate();
			Vehicles[i]->brakeRelease();
		}
		else
		{
			Vehicles[i]->idle();
			Vehicles[i]->brake();
		}
		vec3 fwd = createIonconVector(Vehicles[i]->getVehicle()->getChassisWorldTransform().getBasis().getColumn(0));
		//vec3 D = Spline->getDerivative(t + currentSpeed * 0.0001f).norm();
		//vec3 D = Spline->getDerivative(t + 0.05f).norm();

		int nforward = 5;
		for (int j=0; j<nforward; j++)
		{
		}
		vec3 D = Spline->getDerivative(t + 0.05f).norm();
		vec3 S = Spline->getBinormal(t).norm();
		float steerCurvature = fwd.dot(D) * 0.5f;
		Vehicles[i]->steer(steerCurvature, dt);

		//std::ostringstream str;
		//str << optimalSpeed;
		//Overlays[i]->setText(str.str());

		if (Vehicles[i]->getRenderObject()->getPosition().Y < -200) // magic
			resetCar(Vehicles[i]);
	}

	std::ostringstream ss;
	ss << std::endl;
	ss << "Speed: ";
	ss << abs(floor(Vehicle->getVehicle()->getCurrentSpeedKmHour() * 0.3f));
	ss << " km/h" << std::endl;
	ss << "Lap: " << ln << std::endl;
	ss << "Checkpoints: " << CPController->getCurrentCheckpoint(Vehicle->getRenderObject()) << "/" << CPController->getNumberOfCheckpoints() << std::endl;
	ss << "Time: " << (Timer.getTimeSeconds()) << " s" << std::endl;
	ss << "Lap time: " << (laptime) << " s" << std::endl;
	ss << "Best lap: " << (bestlap) << " s" << std::endl;

	CPController->sort();
	ss << std::endl;
	for (uint i=0; i<CPController->getNumberOfTrackers(); i++)
	{
		SCheckpointTracker* tracker = CPController->getTracker(i);
		ss << (i + 1) << " - " << tracker->Name << " (L" << tracker->LapNum << " C" << tracker->CurrentCheckpoint << ")" << std::endl;
	}

	Overlay->setText(ss.str());

	if (KeyDown[27])
		exit(0);

	if (keyPressed('P'))
		glutFullScreenToggle();

	if (keyPressed('r'))
		resetCar(Vehicle);

	updateKeyStrokes();
}

CCar* CGLUTApplication::addCar(const char* propFile)
{
	SCarProperties props;
	char carMeshStr[100];
	char hullMeshStr[100];
	char wheelMeshStr[100];
	loadCarProperties(propFile, props, carMeshStr, hullMeshStr, wheelMeshStr);
	CMesh* carMesh = new CMesh(carMeshStr);
	CMesh* hullMesh = new CMesh(hullMeshStr);
	CMesh* wheelMesh = new CMesh(wheelMeshStr);

	btCollisionShape* carShape = Physics->generateConvexHullShape(hullMesh);
	CObjectMesh* carObject = Scene->addObjectMesh(carMesh);
	CCar* car = Physics->addCar(props, carObject, carShape, wheelMesh);
	for (int i=0; i<4; i++)
		Scene->addObjectToRoot(car->getWheelObject(i));
	Vehicles.push_back(car);
	//VehiclesClosest.push_back(Scene->addObjectSphere(2));
	COverlayText* over = new COverlayText("", GLUT_BITMAP_HELVETICA_18, vec3(0,5,0), true);
	over->setParent(carObject);
	Overlays.push_back(over);
	return car;
}

void CGLUTApplication::resetCar(CCar* car)
{
	btRigidBody* body = car->getPhysicsObject();
	//float t = Spline->getClosestPoint(car->getRenderObject()->getPosition());
	float t = Spline->getClosestPoint(CPController->getCheckpoint(CPController->getCurrentCheckpoint(car->getRenderObject()) - 1)->Position);
	mat4 basis;
	Spline->getFrameBasis(t, basis);
	Spline->getFrameBasisFromStencil(t, basis, vec3(0,3,0));
	basis = getOpenGLMatrixFromFrameBasis(basis);

	btTransform trans;
	trans.setFromOpenGLMatrix(basis.M);
	body->setCenterOfMassTransform(trans);
	body->setAngularVelocity(btVector3(0,0,0));
	body->setLinearVelocity(btVector3(0,0,0));
}

float CGLUTApplication::getTimeStep()
{
	static unsigned long t1 = Clock.getTimeMicroseconds();
	unsigned long t2 = Clock.getTimeMicroseconds();
	unsigned long dt = t2 - t1;
	t1 = t2;
	if (dt < 1)
		dt++;
	return dt / 1000000.f;
}

void CGLUTApplication::render()
{
	Scene->render();
	if (DoubleBuffering)
		glutSwapBuffers();
}

void CGLUTApplication::mouseMove(int x, int y)
{
	static bool warp = false;
	static int lastX = 100;
	static int lastY = 100;
	int deltaX = x - lastX;
	int deltaY = y - lastY;

	lastX = x;
	lastY = y;

	if (deltaX == 0 && deltaY == 0)
		return;

	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);

	//if (x < 20 || y < 20 || x > w-20 || y > h-20)
	//{
	if (warp)
	{
		lastX = w/2;
		lastY = h/2;
		glutWarpPointer(lastX, lastY);
		warp = false;
	}
	else
		warp = true;

	CameraFPS->mouseLook((float)deltaX, (float)deltaY);
}

bool CGLUTApplication::keyDown(char key)
{
	return KeyDown[key];
}

bool CGLUTApplication::keyUp(char key)
{
	return !KeyDown[key];
}

bool CGLUTApplication::keyPressed(char key)
{
	return KeyDown[key] && !KeyDownPrev[key];
}

bool CGLUTApplication::keyReleased(char key)
{
	return !KeyDown[key] && KeyDownPrev[key];
}

void CGLUTApplication::updateKeyStrokes()
{
	for (int i=0; i<256; i++)
		KeyDownPrev[i] = KeyDown[i];
}

// ------------------------ HANDLERS -----------------------------

void CGLUTApplication::displayFunc()
{
	step();
	render();
}

void CGLUTApplication::idleFunc()
{
	step();
	render();
}

void CGLUTApplication::reshapeFunc(int w, int h)
{
	Camera->recalculateProjection();
}

void CGLUTApplication::keyboardFunc(unsigned char c, int x, int y)
{
	KeyDown[c] = true;
}

void CGLUTApplication::keyboardUpFunc(unsigned char c, int x, int y)
{
	KeyDown[c] = false;
}

void CGLUTApplication::motionFunc(int x, int y)
{
	//mouseMove(x, y);
}

void CGLUTApplication::passiveMotionFunc(int x, int y)
{
	mouseMove(x, y);
}

void CGLUTApplication::specialFunc(int c, int x, int y)
{
}

void CGLUTApplication::mouseFunc(int button, int state, int x, int y)
{
}