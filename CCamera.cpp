#include "CCamera.h"

void CCamera::render()
{
	if (WindowDirty)
		recalculateProjection();

	vec3 p(getPosition());
	gluLookAt(p.X, p.Y, p.Z,
		Focus.X, Focus.Y, Focus.Z,
		0.f, 1.f, 0.f);
}

void CCamera::recalculateProjection()
{
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	glViewport(0, 0, w, h);
	Ratio = w / (float) h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOV, Ratio, Near, Far);
	glMatrixMode(GL_MODELVIEW);
	WindowDirty = false;
}