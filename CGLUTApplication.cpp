#include "CGLUTApplication.h"
#include <stdio.h>
#include <time.h>

#include "CObjectMesh.h"
#include "CObjectShapes.h"
#include "CObjectSkyDome.h"
#include "CObjectCheckpointController.h"
#include "utils.h"
#include "physics.h"
#include "icr_loader.h"
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
		KeyDown[i] = false;

	Scene = new CScene();
	Physics = new CPhysicsWorld();
	ShaderManager = new cwc::glShaderManager();
	//Camera = new CCameraFPS();
	//Camera->setPosition(vec3(15,10,30));
	//Camera->setSpeed(50.f);

	Camera = new CCameraFollower(0);
	Camera->setDistance(20.f);
	Camera->setDisplacement(vec3(0,10,0));
	Camera->setFar(3000.f);
	Camera->setStiffness(50.f);
	Camera->setSpeed(0.2f);

	float v[4] = {1,1,1,1};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, v);
	//glLightf

	Scene->setActiveCamera(Camera);
	Scene->setClearColor(vec3(0.10f, 0.11f, 0.15f));

	// sky box test
	uint skytex = Scene->loadTexture("images/sky3.jpg");
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
	spline = generateRoad(10, 800, 1000, PI * 0.1f);
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
	CMesh* lampMesh = new CMesh("models/lamp01.obj", "models/");
	float tend = (float) spline->getNumberOfControlPoints();
	float dt = tend / 100.f;
	mat4 basis;
	for (float t=0; t<tend; t+=dt)
	{
		spline->getFrameBasis(t, basis);
		float displace = 28;
		basis.M[3] += basis.M[0] * displace;
		basis.M[7] += basis.M[4] * displace;
		basis.M[11] += basis.M[8] * displace;
		CObject* lamp = Scene->addObjectMesh(lampMesh);
		lamp->setTransformation(basis.transpose());
	}

	// add checkpoints
	CPController = new CObjectCheckpointController();
	Scene->addObjectToRoot(CPController); // for animation and querying
	int nnn = 10;
	for (int i=0; i<spline->getNumberOfControlPoints() * nnn; i++)
		CPController->addCheckpoint(SCheckpoint(spline->getPosition(i/(float)nnn), 30.f));

	// put a creepy red sphere above the starting position

	CObject* redSphere = new CObjectMesh(new CMesh("models/startflag.obj", "models/"));
	redSphere->setPosition(spline->getPosition(0) + vec3(0,15,0));
	redSphere->setTexture(0, Scene->loadTexture("images/startflag.png"));
	Scene->addObjectToRoot(redSphere);



	// phong shader
	cwc::glShader* phongShader = ShaderManager->loadfromFile("shaders/phong.vert", "shaders/phong.frag");
	
	// car0 - Chevrolet Corvette
	CMesh* carMesh = new CMesh("models/cars/sls_amg.obj", "models/cars/");
	CMesh* carHull = new CMesh("models/cars/sls_amg_hull.obj", "models/cars/");
	//CMesh* carMesh = new CMesh("models/cars/sls_amg.obj", "models/cars/");
	//CMesh* carHull = new CMesh("models/cars/sls_amg_hull.obj", "models/cars/");
	CMesh* wheelMesh = new CMesh("models/cars/sls_amg_wheel.obj", "models/cars/");
	btCollisionShape* carShape = Physics->generateConvexHullShape(carHull);
	CObjectMesh* carObject = Scene->addObjectMesh(carMesh);
	carObject->setPosition(spline->getPosition(0) + vec3(0,10,0));
	//carObject->setShader(phongShader);
	Camera->setFollowedObject(carObject);

	SCarProperties props;
	props.ChasisObject = carObject;
	props.ChasisShape = carShape;
	props.WheelMesh = wheelMesh;
	props.MaxSteer = 0.2f;
	props.Mass = 0.65f;
	props.SuspensionDamping = 20.f;
	props.SuspensionStiffness = 100.f;
	props.FrictionSlip = 3.4f;
	props.EngineForce = 9.f;
	props.BrakeForce = 0.22f;
	props.WheelPositionBackRight.Y += 2.2;
	props.WheelPositionFrontLeft.Y += 2.3;
	props.WheelPositionBackRight.X += 0.0;
	props.WheelPositionFrontLeft.X -= 0.0;
	props.WheelPositionFrontLeft.Z -= 0.5;
	props.WheelPositionBackRight.Z -= 0.1;

	Vehicle = addCar(props);
	CPController->addObjectTracker(Vehicle->getRenderObject());

	// car1 - Porsche Carrera 911
	/*CMesh* carMesh1 = new CMesh("models/cars/911.obj", "models/cars/");
	CMesh* carHull1 = new CMesh("models/cars/911_hull.obj", "models/cars/");
	btCollisionShape* carShape1 = Physics->generateConvexHullShape(carHull1);
	CObjectMesh* carObject1 = Scene->addObjectMesh(carMesh1);
	carObject1->setPosition(vec3(35,0,100));
	Physics->addDynamicObject(carObject1, carShape1, 2.f);

	// car2 - Ford Mustang GT500
	CMesh* carMesh2 = new CMesh("models/cars/mustang.obj", "models/cars/");
	CMesh* carHull2 = new CMesh("models/cars/mustang_hull.obj", "models/cars/");
	btCollisionShape* carShape2 = Physics->generateConvexHullShape(carHull2);
	CObjectMesh* carObject2 = Scene->addObjectMesh(carMesh2);
	carObject2->setPosition(vec3(38,0,130));
	Physics->addDynamicObject(carObject2, carShape2, 2.f);

	// car3 - Porsche Carrera GT
	CMesh* carMesh3 = new CMesh("models/cars/carreraGT.obj", "models/cars/");
	CMesh* carHull3 = new CMesh("models/cars/carreraGT_hull.obj", "models/cars/");
	btCollisionShape* carShape3 = Physics->generateConvexHullShape(carHull3);
	CObjectMesh* carObject3 = Scene->addObjectMesh(carMesh3);
	carObject3->setPosition(vec3(45,0,100));
	Physics->addDynamicObject(carObject3, carShape3, 2.f);

	// car4 - Ferrari California
	CMesh* carMesh4 = new CMesh("models/cars/california.obj", "models/cars/");
	CMesh* carHull4 = new CMesh("models/cars/california_hull.obj", "models/cars/");
	btCollisionShape* carShape4 = Physics->generateConvexHullShape(carHull4);
	CObjectMesh* carObject4 = Scene->addObjectMesh(carMesh4);
	carObject4->setPosition(vec3(45,0,120));
	Physics->addDynamicObject(carObject4, carShape4, 2.f);*/

	// car5 - Mercedes SLS AMG
	/*CMesh* carMesh5 = new CMesh("models/cars/sls_amg.obj", "models/cars/");
	CMesh* carHull5 = new CMesh("models/cars/sls_amg_hull.obj", "models/cars/");
	btCollisionShape* carShape5 = Physics->generateConvexHullShape(carHull5);
	CObjectMesh* carObject5 = Scene->addObjectMesh(carMesh5);
	carObject5->setPosition(vec3(55,0,150));
	Physics->addDynamicObject(carObject5, carShape5, 2.f);
	carObject5->setShader(phongShader);*/

	// car6 - Mercedes SL 500
	/*CMesh* carMesh6= new CMesh("models/cars/SL500.obj", "models/cars/");
	CMesh* carHull6 = new CMesh("models/cars/SL500_hull.obj", "models/cars/");
	btCollisionShape* carShape6 = Physics->generateConvexHullShape(carHull6);
	CObjectMesh* carObject6 = Scene->addObjectMesh(carMesh6);
	carObject6->setPosition(vec3(65,0,160));
	Physics->addDynamicObject(carObject6, carShape6, 2.f);*/
	
	// ---------------------------------------------------------------------------------------------
	
	//block bigHouse
	/*ObjectMesh* houseObject = Scene->addObjectMesh(houseMesh);
	houseObject->setPosition(vec3(300,0,180));
	houseObject->setShader(phongShader);

	//block hotelBlock
	CMesh* blockMesh = new CMesh("models/architecture/hotelBlock.obj", "models/architecture/");
	CObjectMesh* blockObject = Scene->addObjectMesh(blockMesh);
	blockObject->setPosition(vec3(270,0,-20));
	blockObject->setShader(phongShader);

	//block newYork
	CMesh* newYorkMesh = new CMesh("models/architecture/newYorkStyle.obj", "models/architecture/");
	CObjectMesh* newYorkObject = Scene->addObjectMesh(newYorkMesh);
	newYorkObject->setPosition(vec3(160,50,130));
	Physics->addStaticMeshObject(newYorkObject);
	newYorkObject->setShader(phongShader);

	//block businessBlok
	CMesh* businessMesh = new CMesh("models/architecture/businessBlock.obj", "models/architecture/");
	CObjectMesh* businessObject = Scene->addObjectMesh(businessMesh);
	businessObject->setPosition(vec3(450,20,450));
	businessObject->setRotation(vec3(0,-120,0));
	businessObject->setShader(phongShader);

	//block businessBlok2
	CObjectMesh* businessObject2 = Scene->addObjectMesh(businessMesh);
	businessObject2->setPosition(vec3(50,-10,300));
	businessObject2->setRotation(vec3(0,60,0));
	businessObject2->setShader(phongShader);

	//city
	CMesh* cityMesh = new CMesh("models/architecture/downtown_scenery.obj", "models/architecture/");
	CObjectMesh* cityObject = Scene->addObjectMesh(cityMesh);
	cityObject->setPosition(vec3(0,-50,1600));
	cityObject->setShader(phongShader);*/

	//test generate block
	/*uint startTex = Scene->loadTexture("models/blockGenerator/blockStart2.png");
	uint itemTex = Scene->loadTexture("models/blockGenerator/blockItem2.png");
	blockGenerator* gener = new blockGenerator("models/blockGenerator/blockStart2.obj",
												"models/blockGenerator/blockItem2.obj",
												"models/blockGenerator/blockEnd2.obj",
												"models/blockGenerator/",
												startTex, itemTex, 0);
	std::vector<CObjectMesh*> geneBlockObjVect = gener->generateBlock(5,9,5,vec3(-30,-20,-30));
	for(size_t i=0; i<geneBlockObjVect.size(); i++)
	{
		Scene->addObjectToRoot(geneBlockObjVect[i]);
	}
	geneBlockObjVect = gener->generateBlock(8,11,5,vec3(-30,-20,100));
	for(size_t i=0; i<geneBlockObjVect.size(); i++)
	{
		Scene->addObjectToRoot(geneBlockObjVect[i]);
	}*/

	// physics demo
	/*uint texture = Scene->loadTexture("images/art.png");
	CObjectPlane* plane = new CObjectPlane(20.f, 5);
	plane->setTexture(0, texture);
	Scene->addObjectToRoot(plane);
	btCollisionShape* planeShape = new btStaticPlaneShape(btVector3(0,1,0), 0);
	CPhysicsObject* planeObject = new CPhysicsObject(plane, planeShape, Physics->getWorld(), 0.f);
	planeObject->getRigidBody()->setRestitution(0.7f);

	plane = new CObjectPlane(20.f, 5);
	plane->setRotation(vec3(90.f * DEGTORAD, 0, 0));
	plane->setPosition(vec3(0,20,0));
	plane->setTexture(0, texture);
	Scene->addObjectToRoot(plane);
	planeShape = new btStaticPlaneShape(btVector3(0,0,1), 0);
	planeObject = new CPhysicsObject(plane, planeShape, Physics->getWorld(), 0.f);
	planeObject->getRigidBody()->setRestitution(0.7f);

	btCollisionShape* sphereShape = new btSphereShape(1.f);
	for (int i=0; i<50; i++)
	{
		CObjectSphere* sphere = new CObjectSphere(1.f);
		Scene->addObjectToRoot(sphere);
		sphere->setPosition(vec3((float)(rand()%20), (float)(rand()%10), (float)(rand()%20)));
		CPhysicsObject* sphereObject = new CPhysicsObject(sphere, sphereShape, Physics->getWorld(), 1.f);
		sphereObject->getRigidBody()->setRestitution(0.7f);
	}

	// testing convex hull
	CMesh* gunmesh = new CMesh("models/gunhull.obj", "models/");
	btConvexHullShape* meshshape = Physics->generateConvexHullShape(gunmesh);
	CPhysicsObject* physicsobject = new CPhysicsObject(obj, meshshape, Physics->getWorld(), 3.f);*/
}

void CGLUTApplication::step()
{
	float dt = getTimeStep();
	Physics->getWorld()->stepSimulation(dt, 10);
	Scene->animate(dt);

	//if (KeyDown['w'] || KeyDown['W'])
	//	Camera->moveForward(dt);
	//if (KeyDown['s'] || KeyDown['S'])
	//	Camera->moveBackward(dt);
	//if (KeyDown['a'] || KeyDown['A'])
	//	Camera->moveLeft(dt);
	//if (KeyDown['d'] || KeyDown['D'])
	//	Camera->moveRight(dt);

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
	static clock_t start = clock();
	static clock_t end = clock();
	static uint current = -1;
	uint ln = CPController->getLapNum(Vehicle->getRenderObject());
	if (current != ln)
	{
		end = clock();
		printf("--- Lap time: %5.2fs\n", (end-start)/(float)CLOCKS_PER_SEC);
		start = end;
		printf("--- Lap Number: %u\n", ln);
		current = ln;
	}
	/*static uint ccp = -1;
	uint cp = CPController->getCurrentCheckpoint(Vehicle->getRenderObject());
	if (cp != ccp)
	{
		printf("--- Checkpoint number: %u\n", cp);
		ccp = cp;
	}*/

	if (KeyDown[27])
		exit(0);

	if (KeyDown['p'])
		glutFullScreenToggle();
}

CCar* CGLUTApplication::addCar(SCarProperties& props)
{
	CCar* car = Physics->addCar(props);
	for (int i=0; i<4; i++)
		Scene->addObjectToRoot(car->getWheelObject(i));
	return car;
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

	//Camera->mouseLook((float)deltaX, (float)deltaY);
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
	static uint tex = 0;
	if (tex == 0)
		tex = Scene->loadTexture("images/steelplate.jpg");

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//CObject* obj = new CObjectCube(1.f);
		CObject* obj = new CObjectSphere(1.f);
		Scene->addObjectToRoot(obj);
		obj->setPosition(Camera->getPosition());
		obj->setTexture(0, tex);
		//btCollisionShape* shp = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* shp = new btSphereShape(1.f);
		CPhysicsObject* objP = Physics->addDynamicObject(obj, shp, 1.f);
		objP->getPhysicsObject()->setRestitution(0.7f);
		objP->getPhysicsObject()->setLinearVelocity(createBulletVector(Camera->getFocus() * 50.f));
	}
}