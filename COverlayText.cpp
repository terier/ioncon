#include "COverlayText.h"

COverlayText::COverlayText(const std::string& text, void* font, const vec3& position, bool is3d, const vec3& color) :
CObject(0, position), Text(text), Font(font), Is3D(is3d), Color(color)
{
}

COverlayText::~COverlayText()
{
}

void COverlayText::render()
{
	GLboolean alpha = glIsEnabled(GL_ALPHA_TEST);
	GLboolean ztest = glIsEnabled(GL_DEPTH_TEST);
	GLboolean light = glIsEnabled(GL_LIGHTING);
	GLboolean bcull = glIsEnabled(GL_CULL_FACE);
	if (alpha)
		glDisable(GL_ALPHA_TEST);
	if (ztest)
		;//glDisable(GL_DEPTH_TEST);
	if (light)
		glDisable(GL_LIGHTING);
	if (bcull)
		glDisable(GL_CULL_FACE);

	if (Is3D)
	{
		glPushMatrix();
		transform();
	}
	else
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();    

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		int w = glutGet(GLUT_WINDOW_WIDTH);
		int h = glutGet(GLUT_WINDOW_HEIGHT);
		gluOrtho2D(0, 1, 0, 1);
	}

	vec3 pos = getPosition();
	glBindTexture(GL_TEXTURE_2D, 0);
	glRasterPos2f(pos.X, pos.Y);
	glColor3f(Color.X, Color.Y, Color.Z);
	glutBitmapString(Font, (unsigned char*) Text.c_str());

	if (Is3D)
	{
		CObject::render();
		glPopMatrix();
	}
	else
	{
		glPopMatrix();
		glMatrixMode( GL_MODELVIEW );
		glPopMatrix();
	}

	if (alpha)
		glEnable(GL_ALPHA_TEST);
	if (ztest)
		glEnable(GL_DEPTH_TEST);
	if (light)
		glEnable(GL_LIGHTING);
	if (bcull)
		glEnable(GL_CULL_FACE);
}