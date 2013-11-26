#ifndef CGLUT_APPLICATION_H_INCL
#define CGLUT_APPLICATION_H_INCL

#include "opengl.h"
#include "physics.h"
#include "CEventReceiver.h"
#include "CScene.h"
#include "CCameraFPS.h"

struct SGLUTParameters
{
	SGLUTParameters() : WindowX(0), WindowY(0), WindowW(640), WindowH(480),
		WindowTitle(0), DoubleBuffering(true), FSAA(false)
	{
	}

	int WindowX;
	int WindowY;
	int WindowW;
	int WindowH;
	const char* WindowTitle;
	bool DoubleBuffering;
	bool FSAA;
};

class CGLUTApplication : public CEventReceiver
{
public:
	CGLUTApplication(const SGLUTParameters& param);
	~CGLUTApplication();

	void init();
	void step();
	float getTimeStep();
	void render();
	void mouseMove(int x, int y);

	virtual void displayFunc();
	virtual void idleFunc();
	virtual void mouseFunc(int button, int state, int x, int y);
	virtual void reshapeFunc(int w, int h);
	virtual void keyboardFunc(unsigned char c, int x, int y);
	virtual void keyboardUpFunc(unsigned char c, int x, int y);
	virtual void specialFunc(int c, int x, int y);
	virtual void motionFunc(int x, int y);
	virtual void passiveMotionFunc(int x, int y);

private:
	bool DoubleBuffering;

	CScene* Scene;
	CPhysicsWorld* Physics;
	CCameraFPS* Camera;
	btClock Clock;

	bool KeyDown[256];
};

#endif