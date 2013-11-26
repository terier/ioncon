#ifndef CPHYSICS_WORLD_H_INCL
#define CPHYSICS_WORLD_H_INCL

#include "physics.h"

class CPhysicsWorld
{
public:
	CPhysicsWorld();
	~CPhysicsWorld();

	btDiscreteDynamicsWorld* getWorld() { return World; }

private:
	btBroadphaseInterface* Broadphase;
	btDefaultCollisionConfiguration* Configuration;
	btCollisionDispatcher* Dispatcher;
	btSequentialImpulseConstraintSolver* Solver;
	btDiscreteDynamicsWorld* World;
};

#endif