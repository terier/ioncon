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

	float pos[4] = {500, 1000, 500, 1};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	Root.render();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();    

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	gluOrtho2D(0, w, 0, h);

	for (std::list<COverlayText*>::iterator it = Overlays.begin(); it != Overlays.end(); it++)
	{
		COverlayText* overlay = *it;
		if (!overlay->is3D())
		{
			vec3 pos = overlay->getPosition();
			glRasterPos2f(pos.X, pos.Y);
			glutBitmapString(overlay->getFont(), (unsigned char*) overlay->getText().c_str());
		}
	}

	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
}

void CScene::animate(float dt)
{
	Root.animate(dt);
	if (ActiveCam)
		ActiveCam->animate(dt);
}

uint CScene::loadTexture(const char* fname, bool mipmaps)
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
		if (mipmaps)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		if (mipmaps)
			glGenerateMipmap(GL_TEXTURE_2D);
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
