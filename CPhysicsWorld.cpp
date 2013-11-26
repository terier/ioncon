#include "CPhysicsWorld.h"
#include <stdio.h>

CPhysicsWorld::CPhysicsWorld()
{
	printf("Creating PHYSICS world...\n");
	Broadphase = new btDbvtBroadphase();
	Configuration = new btDefaultCollisionConfiguration();
	Dispatcher = new btCollisionDispatcher(Configuration);
	Solver = new btSequentialImpulseConstraintSolver();
	World = new btDiscreteDynamicsWorld(Dispatcher, Broadphase, Solver, Configuration);
	World->setGravity(btVector3(0, -10, 0));
}

CPhysicsWorld::~CPhysicsWorld()
{
	printf("Destroying PHYSICS world...\n");
	delete World;
	delete Solver;
	delete Configuration;
	delete Dispatcher;
	delete Broadphase;
}