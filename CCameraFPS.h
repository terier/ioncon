#ifndef CCAMERA_FPS_H_INCL
#define CCAMERA_FPS_H_INCL

#include "CCamera.h"

class CCameraFPS : public CCamera
{
public:
	CCameraFPS(float speed = 10.f, float rotateSpeed = 0.0001f, float rotateDamping = 10.f,
		float fov = 60.f, float near_ = 0.5f, float far_ = 5000.f,
		const vec3& pos = vec3(), const vec3& rot = vec3());

	float getDir() const { return Dir; }
	float getZDir() const { return ZDir; }
	float getSpeed() const { return Speed; }
	float getRotateSpeed() const { return RotateSpeed; }
	float getRotateDamping() const { return RotateDamping; }

	void setDir(float d);
	void setZDir(float d);
	void setSpeed(float s) { Speed = s; }
	void setRotateSpeed(float s) { RotateSpeed = s; }
	void setRotateDamping(float d) { RotateDamping = d; }

	vec3 getFocus() const;
	void setFocus(const vec3& focus);

	void moveForward(float dt);
	void moveBackward(float dt);
	void moveLeft(float dt);
	void moveRight(float dt);

	void mouseLook(float dx, float dy);
	void recalculateFocus();

	virtual void animate(float dt);

private:
	float Dir;
	float ZDir;
	float Speed;
	float RotateSpeed;
	float RotateDamping;
	float DX, DY;
};

#endif