#include "CObjectSkyDome.h"

CObjectSkyDome::CObjectSkyDome(uint diffuseTex, uint cloudTex) :
CloudTexture(cloudTex)
{
	Sphere = gluNewQuadric();
	gluQuadricDrawStyle(Sphere, GLU_FILL);
	gluQuadricTexture(Sphere, GL_TRUE);
	gluQuadricNormals(Sphere, GLU_SMOOTH);

	setTexture(diffuseTex);
	setRotation(vec3(90.f * DEGTORAD, 0, 0));
}

CObjectSkyDome::~CObjectSkyDome()
{
	gluDeleteQuadric(Sphere);
}

void CObjectSkyDome::render()
{
	GLboolean alpha = glIsEnabled(GL_ALPHA_TEST);
	GLboolean ztest = glIsEnabled(GL_DEPTH_TEST);
	GLboolean light = glIsEnabled(GL_LIGHTING);
	GLboolean bcull = glIsEnabled(GL_CULL_FACE);
	if (alpha)
		glDisable(GL_ALPHA_TEST);
	if (ztest)
		glDisable(GL_DEPTH_TEST);
	if (light)
		glDisable(GL_LIGHTING);
	if (bcull)
		glDisable(GL_CULL_FACE);

	glPushMatrix();
	transform();
	glBindTexture(GL_TEXTURE_2D, getTexture());
	gluSphere(Sphere, 100, 32, 16);
	CObject::render();
	glPopMatrix();

	if (alpha)
		glEnable(GL_ALPHA_TEST);
	if (ztest)
		glEnable(GL_DEPTH_TEST);
	if (light)
		glEnable(GL_LIGHTING);
	if (bcull)
		glEnable(GL_CULL_FACE);
}