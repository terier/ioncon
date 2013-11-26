#include "CCameraFPS.h"
#include "utils.h"

CCameraFPS::CCameraFPS(float speed, float rotateSpeed,
	float fov, float near_, float far_,
	const vec3& pos, const vec3& rot) :
CCamera(vec3(), 1.f, fov, near_, far_, 0, pos, rot),
Speed(speed), RotateSpeed(rotateSpeed), DX(0), DY(0)
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

void CCameraFPS::moveForward()
{
	vec3 p = getPosition();
	p.X += Speed * cos(Dir) * cos(ZDir);
	p.Z += Speed * sin(Dir) * cos(ZDir);
	p.Y += Speed * sin(ZDir);
	setPosition(p);
	recalculateFocus();
}

void CCameraFPS::moveBackward()
{
	vec3 p = getPosition();
	p.X -= Speed * cos(Dir) * cos(ZDir);
	p.Z -= Speed * sin(Dir) * cos(ZDir);
	p.Y -= Speed * sin(ZDir);
	setPosition(p);
	recalculateFocus();
}

void CCameraFPS::moveLeft()
{
	vec3 p = getPosition();
	p.X += Speed * sin(Dir);
	p.Z -= Speed * cos(Dir);
	setPosition(p);
	recalculateFocus();
}

void CCameraFPS::moveRight()
{
	vec3 p = getPosition();
	p.X -= Speed * sin(Dir);
	p.Z += Speed * cos(Dir);
	setPosition(p);
	recalculateFocus();
}

void CCameraFPS::mouseLook(float dx, float dy)
{
	DX += dx;
	DY += dy;
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
	DX *= 1.f - dt * 10.f;
	DY *= 1.f - dt * 10.f;

	Dir += RotateSpeed * DX;
	if (Dir > 360.f * DEGTORAD) Dir -= 360.f * DEGTORAD;
	if (Dir < 0.f) Dir += 360.f * DEGTORAD;
	ZDir -= RotateSpeed * DY;
	if (ZDir > 89.f * DEGTORAD) ZDir = 89.f * DEGTORAD;
	if (ZDir < -89.f * DEGTORAD) ZDir = -89.f * DEGTORAD;
	recalculateFocus();
}