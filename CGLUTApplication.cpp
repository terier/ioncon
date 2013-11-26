#include "CGLUTApplication.h"
#include <stdio.h>

#include "CObjectShapes.h"
#include "CObjectMotionState.h"
#include "utils.h"

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
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(100,100);

	for (int i=0; i<256; i++)
		KeyDown[i] = false;

	Scene = new CScene();
	Physics = new CPhysicsWorld();
	Camera = new CCameraFPS();
	Scene->setActiveCamera(Camera);

	Camera->setPosition(vec3(15,10,30));
	Camera->setFocus(vec3(10,0,10));

	CObject* obj2 = new CObjectPlane(20.f, 10);
	Scene->addObjectToRoot(obj2);

	btDynamicsWorld* dynamicsWorld = Physics->getWorld();
	dynamicsWorld->setGravity(btVector3(0,-50,0));
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
    btCollisionShape* fallShape = new btSphereShape(1);
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
    btRigidBody::btRigidBodyConstructionInfo
            groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
	groundRigidBodyCI.m_restitution = 0.7f;
	groundRigidBodyCI.m_friction = 0.8f;
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    dynamicsWorld->addRigidBody(groundRigidBody);

	btVector3 inertia;
	fallShape->calculateLocalInertia(1.f, inertia);
	for (int i=0; i<70; i++)
	{
		CObject* object = new CObjectSphere(1.f);
		Scene->addObjectToRoot(object);
		btMotionState* ms = new CObjectMotionState(btTransform(btQuaternion(0,0,0,1),
			btVector3(rand() % 20, 5 + rand() % 20, rand() % 20)), object);
			//btVector3(10,1,10)), object);
		btRigidBody::btRigidBodyConstructionInfo ci(1.f, ms, fallShape, inertia);
		ci.m_restitution = 0.7f;
		ci.m_friction = 0.5f;
		btRigidBody* body = new btRigidBody(ci);
		body->applyImpulse(btVector3(0,2,0), btVector3(rand()%2,0,rand()%2));
		dynamicsWorld->addRigidBody(body);
	}
}

void CGLUTApplication::step()
{
	float dt = getTimeStep();
	Physics->getWorld()->stepSimulation(dt, 10);
	Scene->animate(dt);

	if (KeyDown['w'])
		Camera->moveForward();
	if (KeyDown['s'])
		Camera->moveBackward();
	if (KeyDown['a'])
		Camera->moveLeft();
	if (KeyDown['d'])
		Camera->moveRight();
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

	if (x < 20 || y < 20 || x > w-20 || y > h-20)
	{
		lastX = w/2;
		lastY = h/2;
		glutWarpPointer(lastX, lastY);
	}

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
}