#include "CScene.h"
#include "opengl.h"

CScene::CScene() :
	ActiveCam(0)
{
}

void CScene::addObjectToRoot(CObject* obj)
{
	obj->setParent(&Root);
}

void CScene::render()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1,1,1,1);

	if (ActiveCam)
		ActiveCam->render();

	Root.render();
}

void CScene::animate(float dt)
{
	Root.animate(dt);
	if (ActiveCam)
		ActiveCam->animate(dt);
}