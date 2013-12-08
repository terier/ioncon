#ifndef COBJECT_SKY_DOME_H_INCL
#define COBJECT_SKY_DOME_H_INCL

#include "CObject.h"

class CObjectSkyDome : public CObject
{
public:
	CObjectSkyDome(uint diffuseTex, uint cloudTex = 0);
	virtual ~CObjectSkyDome();

	uint getCloudTexture() const { return CloudTexture; }
	void setCloudTexture(uint tex) { CloudTexture = tex; }

	virtual void render();

private:
	uint CloudTexture;
	GLUquadric* Sphere;
};

#endif