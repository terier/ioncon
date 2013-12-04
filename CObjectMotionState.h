#ifndef COBJECT_MOTION_STATE_H_INCL
#define COBJECT_MOTION_STATE_H_INCL

#include <btBulletDynamicsCommon.h>
#include "CObject.h"
#include "utils.h"

class CObjectMotionState : public btMotionState
{
public:
	CObjectMotionState(const btTransform& transform, CObject* object) :
	  Object(object), Transform(transform)
	{
	}

	virtual ~CObjectMotionState()
	{
	}

	void setObject(CObject* object)
	{
		Object = object;
	}

	virtual void getWorldTransform(btTransform& wt) const
	{
		wt = Transform;
	}

	virtual void setWorldTransform(const btTransform &wt)
	{
		if (Object)
		{
			/*btQuaternion q = wt.getRotation();
			float w, x, y, z;
			w = q.getW();
			x = q.getX();
			y = q.getY();
			z = q.getZ();
			vec3 rot(
				atan2(2.f*(w*x+y*z), 1.f - 2.f*(x*x+y*y)),
				-asin(2.f*(w*y-z*x)),
				atan2(2.f*(w*z+x*y), 1.f - 2.f*(y*y+z*z))
			);
			btVector3 r = wt.getOrigin();
			vec3 pos(r.getX(), r.getY(), r.getZ());
			Object->setRotation(rot * RADTODEG);
			Object->setPosition(pos);*/
			wt.getOpenGLMatrix(Object->getTransformationPointer().M);
		}
	}

private:
	CObject* Object;
	btTransform Transform;
};

#endif