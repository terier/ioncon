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
	World->setGravity(btVector3(0, -50, 0));
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

btConvexHullShape* CPhysicsWorld::generateConvexHullShape(CMesh* mesh)
{
	btConvexHullShape* shape = new btConvexHullShape();
	const ShapeVector& shapes = mesh->getShapes();
	for (uint i=0; i<shapes.size(); i++)
	{
		const FloatVector& points = shapes[i].mesh.positions;
		for (uint j=0; j<points.size(); j+=3)
			shape->addPoint(btVector3(points[j], points[j+1], points[j+2]));
	}
	return shape;
}