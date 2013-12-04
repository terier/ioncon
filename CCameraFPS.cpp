#include "CCameraFPS.h"
#include "utils.h"

CCameraFPS::CCameraFPS(float speed, float rotateSpeed, float rotateDamping,
	float fov, float near_, float far_,
	const vec3& pos, const vec3& rot) :
CCamera(vec3(), 1.f, fov, near_, far_, 0, pos, rot),
Speed(speed), RotateSpeed(rotateSpeed), RotateDamping(rotateDamping),
DX(0), DY(0)
{
	recalculateProjection();
	recalculateFocus();
}

void CCameraFPS::setDir(float d)
{
	Dir = fmod(d, 360.f * DEGTORAD);
	recalculateFocus();
}

void CCameraFPS::setZDir(float d)
{
	ZDir = clamp(d, -89.f * DEGTORAD, 89.f * DEGTORAD);
	recalculateFocus();
}

void CCameraFPS::moveForward(float dt)
{
	vec3 p = getPosition();
	p += getFocus() * (Speed * dt);
	setPosition(p);
	recalculateFocus();
}

void CCameraFPS::moveBackward(float dt)
{
	vec3 p = getPosition();
	p -= getFocus() * (Speed * dt);
	setPosition(p);
	recalculateFocus();
}

void CCameraFPS::moveLeft(float dt)
{
	vec3 p = getPosition();
	p.X += Speed * sin(Dir) * dt;
	p.Z -= Speed * cos(Dir) * dt;
	setPosition(p);
	recalculateFocus();
}

void CCameraFPS::moveRight(float dt)
{
	vec3 p = getPosition();
	p.X -= Speed * sin(Dir) * dt;
	p.Z += Speed * cos(Dir) * dt;
	setPosition(p);
	recalculateFocus();
}

void CCameraFPS::mouseLook(float dx, float dy)
{
	DX += dx * RotateSpeed;
	DY += dy * RotateSpeed;
}

void CCameraFPS::recalculateFocus()
{
	vec3 p = getPosition();
	vec3 f(
		p.X + cos(Dir) * cos(ZDir),
		p.Y + sin(ZDir),
		p.Z + sin(Dir) * cos(ZDir)
	);
	CCamera::setFocus(f);
}

void CCameraFPS::setFocus(const vec3& focus)
{
	CCamera::setFocus(focus);
	vec3 d = focus - getPosition();
	Dir = atan2(d.Z, d.X);
	ZDir = atan(d.Y / sqrt(d.X * d.X + d.Z * d.Z));
}

void CCameraFPS::animate(float dt)
{
	// Euler-type ODE solver
	//DX *= 1.f - dt * 10.f;
	//DY *= 1.f - dt * 10.f;

	// exact solution
	DX *= exp(-RotateDamping * dt);
	DY *= exp(-RotateDamping * dt);

	Dir += DX;
	if (Dir > 360.f * DEGTORAD) Dir -= 360.f * DEGTORAD;
	if (Dir < 0.f) Dir += 360.f * DEGTORAD;
	ZDir -= DY;
	if (ZDir > 89.f * DEGTORAD) ZDir = 89.f * DEGTORAD;
	if (ZDir < -89.f * DEGTORAD) ZDir = -89.f * DEGTORAD;

	recalculateFocus();
}

vec3 CCameraFPS::getFocus() const
{
	return vec3(cos(Dir) * cos(ZDir),
		sin(ZDir),
		sin(Dir) * cos(ZDir));
}