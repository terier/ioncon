#include "CPhysicsObject.h"
#include "CObjectMotionState.h"

CPhysicsObject::CPhysicsObject(CObject* object, btCollisionShape* shape, btDynamicsWorld* world, float mass)
{
	Object = object;
	World = world;

	btVector3 inertia;
	shape->calculateLocalInertia(1.f, inertia);
	
	vec3 position = Object->getPosition();
	btVector3 pos(position.X, position.Y, position.Z);

	btMotionState* ms = new CObjectMotionState(btTransform(btQuaternion(0,0,0,1), pos), Object);
	
	RBody = new btRigidBody(mass, ms, shape, inertia);

	world->addRigidBody(RBody);
}

CPhysicsObject::~CPhysicsObject() {
	World->removeRigidBody(RBody);
	delete RBody;
	delete Object;
}

CObject* CPhysicsObject::getObject() { return Object; }

btRigidBody* CPhysicsObject::getRigidBody() { return RBody; }