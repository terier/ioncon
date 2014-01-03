#ifndef CPHYSICS_OBJECT_H_INCL
#define CPHYSICS_OBJECT_H_INCL

#include "CObject.h"
#include <btBulletDynamicsCommon.h>

class CPhysicsObject : public btMotionState
{
public:
	CPhysicsObject(CObject* object, btCollisionShape* shape, float mass);
	virtual ~CPhysicsObject();

	virtual void getWorldTransform(btTransform& wt) const;
	btVector3 getInertia() const { return Inertia; }

	virtual void setWorldTransform(const btTransform& wt);

	CObject* getRenderObject() { return Object; }
	btRigidBody* getPhysicsObject() { return RBody; }

private:
	CObject* Object;
	btRigidBody* RBody;
	btTransform Transform;
	btVector3 Inertia;
};

#endif