#include "CGLUTApplication.h"
#include <stdio.h>

#include "CObjectSpline.h"
#include "CObjectMesh.h"
#include "CObjectShapes.h"
#include "CObjectMotionState.h"
#include "utils.h"
#include "physics.h"
#include "icr_loader.h"

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
	glutInitDisplayMode(mode);
	glutInitWindowPosition(param.WindowX, param.WindowY);
	glutInitWindowSize(param.WindowW, param.WindowH);
	if (param.WindowTitle)
		glutCreateWindow(param.WindowTitle);
	else
		glutCreateWindow("");

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
	//glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5f);

	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(100,100);

	for (int i=0; i<256; i++)
		KeyDown[i] = false;

	Scene = new CScene();
	Physics = new CPhysicsWorld();
	Camera = new CCameraFPS();
	Scene->setActiveCamera(Camera);

	Camera->setPosition(vec3(15,10,30));
	Camera->setSpeed(50.f);

	CMesh* m = new CMesh("models/gun.obj", "models/");
	CObjectMesh* obj = new CObjectMesh(m);
	obj->setPosition(vec3(10,5,10));
	obj->setRotation(vec3(10,10,10));
	Scene->addObjectToRoot(obj);

	uint guntex = Scene->loadTexture("images/guntex.png");
	obj->setTexture(guntex);

	CObjectSpline* objSpline = loadRoad("models/testroad.icr", Scene);
	Scene->addObjectToRoot(objSpline);
	uint roadtex = Scene->loadTexture("images/roadtex.png");
	objSpline->setTexture(roadtex);

	// physics demo
	uint texture = Scene->loadTexture("images/art.png");
	CObjectPlane* plane = new CObjectPlane(20.f, 5);
	plane->setTexture(texture);
	Scene->addObjectToRoot(plane);
	btCollisionShape* planeShape = new btStaticPlaneShape(btVector3(0,1,0), 0);
	CPhysicsObject* planeObject = new CPhysicsObject(plane, planeShape, Physics->getWorld(), 0.f);
	planeObject->getRigidBody()->setRestitution(0.7f);

	plane = new CObjectPlane(20.f, 5);
	plane->setRotation(vec3(90.f * DEGTORAD, 0, 0));
	plane->setPosition(vec3(0,20,0));
	plane->setTexture(texture);
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
	CPhysicsObject* physicsobject = new CPhysicsObject(obj, meshshape, Physics->getWorld(), 3.f);
}

void CGLUTApplication::step()
{
	float dt = getTimeStep();
	Physics->getWorld()->stepSimulation(dt, 10);
	Scene->animate(dt);

	if (KeyDown['w'])
		Camera->moveForward(dt);
	if (KeyDown['s'])
		Camera->moveBackward(dt);
	if (KeyDown['a'])
		Camera->moveLeft(dt);
	if (KeyDown['d'])
		Camera->moveRight(dt);
	if (KeyDown[27])
		exit(0);
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

	Camera->mouseLook((float)deltaX, (float)deltaY);
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
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//CObject* obj = new CObjectCube(1.f);
		CObject* obj = new CObjectSphere(1.f);
		Scene->addObjectToRoot(obj);
		obj->setPosition(Camera->getPosition());
		//btCollisionShape* shp = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* shp = new btSphereShape(1.f);
		CPhysicsObject* objP = new CPhysicsObject(obj, shp, Physics->getWorld(), 1.f);
		objP->getRigidBody()->setRestitution(0.7f);
		objP->getRigidBody()->setLinearVelocity(createBulletVector(Camera->getFocus() * 50.f));
	}
}