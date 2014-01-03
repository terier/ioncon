#include "CCameraFollower.h"

CCameraFollower::CCameraFollower(CObject* followed,
	float distance, float stiffness, float speed,
	const vec3& displacement,
	float fov, float near_, float far_,
	const vec3& pos, const vec3& rot) :
CCamera(vec3(), 1.f, fov, near_, far_, 0, pos, rot),
FollowedObject(followed), Displacement(displacement),
Distance(distance), Stiffness(stiffness), Speed(speed)
{
}

void CCameraFollower::animate(float dt)
{
	if (!FollowedObject)
		return;

	vec3 D(FollowedObject->getPosition() + Displacement - getPosition());
	float Dlen = D.len();
	Dlen -= Distance;
	Dlen *= Speed * exp(-Stiffness * dt);
	setPosition(getPosition() + D.norm() * Dlen);

	setFocus(FollowedObject->getPosition());
}