#ifndef CSCENE_H_INCL
#define CSCENE_H_INCL

#include "CObject.h"
#include "CCamera.h"
#include "opengl.h"

typedef unsigned int uint;

class CScene
{
public:
	CScene();

	void addObjectToRoot(CObject* obj);

	CCamera* getActiveCamera() const { return ActiveCam; }
	vec3 getAmbient() const { return Ambient; }

	void setActiveCamera(CCamera* cam) { ActiveCam = cam; }
	void setAmbient(const vec3& amb) { Ambient = amb; }

	void render();
	void animate(float dt);

private:
	CObject Root;
	CCamera* ActiveCam;
	vec3 Ambient;
};

#endif