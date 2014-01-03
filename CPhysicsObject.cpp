#include "CPhysicsObject.h"

CPhysicsObject::CPhysicsObject(CObject* object, btCollisionShape* shape, float mass) :
Object(object)
{
	btVector3 inertia;
	shape->calculateLocalInertia(mass, inertia);
	Transform.setFromOpenGLMatrix(Object->getTransformationPointer().M);
	RBody = new btRigidBody(mass, this, shape, inertia);
}

CPhysicsObject::~CPhysicsObject()
{
	delete RBody;
	delete Object;
}

void CPhysicsObject::getWorldTransform(btTransform& wt) const
{
	wt = Transform;
}

void CPhysicsObject::setWorldTransform(const btTransform& wt)
{
	if (Object)
	{
		wt.getOpenGLMatrix(Object->getTransformationPointer().M);
		Object->setPositionNoUpdate(createIonconVector(wt.getOrigin()));
	}
}