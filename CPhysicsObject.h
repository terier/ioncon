#ifndef CPHYSICS_OBJECT_H_INCL
#define CPHYSICS_OBJECT_H_INCL

#include "CObject.h"
#include "physics.h"

class CPhysicsObject
{
public:
	CPhysicsObject(CObject* object, btCollisionShape* shape, btDynamicsWorld* world, float mass);
	~CPhysicsObject();
private:
	CObject* Object;
	btRigidBody* RBody;
};

#endif