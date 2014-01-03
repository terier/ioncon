#ifndef CCAMERA_FOLLOWER_H_INCL
#define CCAMERA_FOLLOWER_H_INCL

#include "CCamera.h"

class CCameraFollower : public CCamera
{
public:
	CCameraFollower(CObject* followed,
		float distance = 10.f, float stiffness = 1.f, float speed = 0.001f,
		const vec3& displacement = vec3(),
		float fov = 60.f, float near_ = 0.5f, float far_ = 5000.f,
		const vec3& pos = vec3(), const vec3& rot = vec3());

	CObject* getFollowedObject() const { return FollowedObject; }
	vec3 getDisplacement() const { return Displacement; }
	float getDistance() const { return Distance; }
	float getStiffness() const { return Stiffness; }
	float getSpeed() const { return Speed; }

	void setFollowedObject(CObject* followed) { FollowedObject = followed; }
	void setDisplacement(const vec3& displacement) { Displacement = displacement; }
	void setDistance(float distance) { Distance = distance; }
	void setStiffness(float stiffness) { Stiffness = stiffness; }
	void setSpeed(float speed) { Speed = speed; }

	virtual void animate(float dt);

private:
	CObject* FollowedObject;
	vec3 Displacement;
	float Distance;
	float Stiffness;
	float Speed;
};

#endif