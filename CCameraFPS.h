#ifndef CCAMERA_FPS_H_INCL
#define CCAMERA_FPS_H_INCL

#include "CCamera.h"

class CCameraFPS : public CCamera
{
public:
	CCameraFPS(float speed = 0.01f, float rotateSpeed = 0.0001f,
		float fov = 60.f, float near_ = 0.5f, float far_ = 500.f,
		const vec3& pos = vec3(), const vec3& rot = vec3());

	float getDir() const { return Dir; }
	float getZDir() const { return ZDir; }
	float getSpeed() const { return Speed; }
	float getRotateSpeed() const { return RotateSpeed; }

	void setDir(float d);
	void setZDir(float d);
	void setSpeed(float s) { Speed = s; }
	void setRotateSpeed(float s) { RotateSpeed = s; }
	void setFocus(const vec3& focus);

	void moveForward();
	void moveBackward();
	void moveLeft();
	void moveRight();

	void mouseLook(float dx, float dy);
	void recalculateFocus();

	virtual void animate(float dt);

private:
	float Dir;
	float ZDir;
	float Speed;
	float RotateSpeed;
	float DX, DY;
};

#endif