#include "CScene.h"
#include "opengl.h"
#include "stb_image.h"

CScene::CScene() :
	ActiveCam(0), SkyDome(0)
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
	glClearColor(ClearColor.X, ClearColor.Y, ClearColor.Z, 1);

	if (ActiveCam)
	{
		ActiveCam->render();
		if (SkyDome)
		{
			SkyDome->setPosition(ActiveCam->getPosition());
			SkyDome->render();
		}
	}

	// TODO this is wrong ....
	/*vec3 p = ActiveCam->getPosition();
	float pos[3];
	pos[0] = p.X;
	pos[1] = p.Y;
	pos[2] = p.Z;
	glLightfv(GL_LIGHT0, GL_POSITION, pos);*/

	Root.render();
}

void CScene::animate(float dt)
{
	Root.animate(dt);
	if (ActiveCam)
		ActiveCam->animate(dt);
}

uint CScene::loadTexture(const char* fname)
{
	int w, h, n;
	unsigned char* data = stbi_load(fname, &w, &h, &n, 4);
	if (!data)
	{
		printf("Cannot load image %s\n", fname);
		return 0;
	}
	else
	{
		uint img;
		glGenTextures(1, &img);
		glBindTexture(GL_TEXTURE_2D, img);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
		return img;
	}
}

CObjectMesh* CScene::addObjectMesh(CMesh* mesh, bool physics)
{
	CObjectMesh* object = new CObjectMesh(mesh);
	addObjectToRoot(object);
	// TODO physics object
	return object;
}

CObjectSphere* CScene::addObjectSphere(float radius, bool physics)
{
	CObjectSphere* object = new CObjectSphere(radius);
	addObjectToRoot(object);
	// TODO physics object
	return object;
}

CObjectCube* CScene::addObjectCube(float size, bool physics)
{
	CObjectCube* object = new CObjectCube(size);
	addObjectToRoot(object);
	// TODO physics object
	return object;
}
