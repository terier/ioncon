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

btConvexHullShape* CPhysicsWorld::generateConvexHullShape(const CMesh* mesh)
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

btBvhTriangleMeshShape* CPhysicsWorld::generateStaticMeshShape(const CMesh* mesh)
{
	btTriangleMesh* bulletmesh = new btTriangleMesh();
	const ShapeVector& shapes = mesh->getShapes();
	for (ShapeVector::const_iterator shape = shapes.begin(); shape != shapes.end(); shape++)
	{
		const mesh_t& mesh = shape->mesh;
		const FloatVector& pos = mesh.positions;
		const UintVector& ind = mesh.indices;
		for (size_t i=0; i<ind.size(); i+=3)
		{
			bulletmesh->addTriangle(
				btVector3(pos[3*ind[i]], pos[3*ind[i]+1], pos[3*ind[i]+2]),
				btVector3(pos[3*ind[i+1]], pos[3*ind[i+1]+1], pos[3*ind[i+1]+2]),
				btVector3(pos[3*ind[i+2]], pos[3*ind[i+2]+1], pos[3*ind[i+2]+2])
			);
		}
	}
	btBvhTriangleMeshShape* shape = new btBvhTriangleMeshShape(bulletmesh, true);
	return shape;
}

CPhysicsObject* CPhysicsWorld::addDynamicObject(CObject* object, btCollisionShape* shape, float mass)
{
	CPhysicsObject* pobj = new CPhysicsObject(object, shape, mass);
	World->addRigidBody(pobj->getPhysicsObject());
	return pobj;
}

CPhysicsObject* CPhysicsWorld::addConvexHullObject(CObjectMesh* object, float mass)
{
	btCollisionShape* shape = generateConvexHullShape(object->getMesh());
	return addDynamicObject(object, shape, mass);
}

CPhysicsObject* CPhysicsWorld::addStaticMeshObject(CObjectMesh* object)
{
	btCollisionShape* shape = generateStaticMeshShape(object->getMesh());
	return addDynamicObject(object, shape, 0);
}