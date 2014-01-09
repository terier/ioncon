#ifndef CSCENE_H_INCL
#define CSCENE_H_INCL

#include <list>

#include "CObject.h"
#include "CObjectMesh.h"
#include "CObjectShapes.h"
#include "CObjectSkyDome.h"
#include "CCamera.h"
#include "COverlayText.h"
#include "opengl.h"

typedef unsigned int uint;

class CScene
{
public:
	CScene();

	void addObjectToRoot(CObject* obj);
	void addOverlayText(COverlayText* overlay);

	CCamera* getActiveCamera() const { return ActiveCam; }
	vec3 getAmbient() const { return Ambient; }
	vec3 getClearColor() const { return ClearColor; }
	CObjectSkyDome* getSkyDome() const { return SkyDome; }

	void setActiveCamera(CCamera* cam) { ActiveCam = cam; }
	void setAmbient(const vec3& amb) { Ambient = amb; }
	void setClearColor(const vec3& col) { ClearColor = col; }
	void setSkyDome(CObjectSkyDome* sky) { SkyDome = sky; }

	void render();
	void animate(float dt);

	unsigned int loadTexture(const char* fname, bool mipmaps = false);

	CObjectMesh* addObjectMesh(CMesh* mesh, bool physics = false);
	CObjectSphere* addObjectSphere(float radius, bool physics = false);
	CObjectCube* addObjectCube(float size, bool physics = false);

private:
	CObject Root;
	CObjectSkyDome* SkyDome;
	CCamera* ActiveCam;
	std::list<COverlayText*> Overlays;
	vec3 Ambient;
	vec3 ClearColor;
};

#endif