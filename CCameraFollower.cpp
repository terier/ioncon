#include "CCameraFollower.h"
#include "CObjectShapes.h"

CCameraFollower::CCameraFollower(CObject* followed,
	float distance, float stiffness, float speed,
	const vec3& displacement, const vec3& viewDisplacement,
	float fov, float near_, float far_,
	const vec3& pos, const vec3& rot) :
CCamera(vec3(), 1.f, fov, near_, far_, 0, pos, rot),
FollowedObject(followed), Distance(distance),
Stiffness(stiffness), Speed(speed)
{
	ViewObject = new CObject(followed, displacement);
	DisplacementObject = new CObject(ViewObject, viewDisplacement);
}

CCameraFollower::~CCameraFollower()
{
	delete ViewObject;
	delete DisplacementObject;
}

void CCameraFollower::setFollowedObject(CObject* followed)
{
	FollowedObject = followed;
	ViewObject->setParent(FollowedObject);
	DisplacementObject->setParent(ViewObject);
}

void CCameraFollower::animate(float dt)
{
	if (!FollowedObject)
		return;

	vec3 D(ViewObject->getAbsolutePosition() - getPosition());
	float Dlen = D.len();
	Dlen -= Distance;
	Dlen *= Speed * exp(-Stiffness * dt);
	setPosition(getPosition() + D.norm() * Dlen);

	setFocus(DisplacementObject->getAbsolutePosition());
}