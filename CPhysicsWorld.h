#ifndef CPHYSICS_WORLD_H_INCL
#define CPHYSICS_WORLD_H_INCL

#include "CMesh.h"
#include "CObjectMesh.h"
#include "CPhysicsObject.h"
#include "CCar.h"

class CPhysicsWorld
{
public:
	CPhysicsWorld();
	~CPhysicsWorld();

	btDiscreteDynamicsWorld* getWorld() { return World; }

	CPhysicsObject* addDynamicObject(CObject* object, btCollisionShape* shape, float mass);
	CPhysicsObject* addConvexHullObject(CObjectMesh* object, float mass);
	CPhysicsObject* addStaticMeshObject(CObjectMesh* object);
	CCar* addCar(SCarProperties& props, CObject* carObject, btCollisionShape* carShape, CMesh* wheelMesh);

	btConvexHullShape* generateConvexHullShape(const CMesh* mesh);
	btBvhTriangleMeshShape* generateStaticMeshShape(const CMesh* mesh);

private:
	btBroadphaseInterface* Broadphase;
	btDefaultCollisionConfiguration* Configuration;
	btCollisionDispatcher* Dispatcher;
	btSequentialImpulseConstraintSolver* Solver;
	btDiscreteDynamicsWorld* World;
	btVehicleRaycaster* Raycaster;
};

#endif