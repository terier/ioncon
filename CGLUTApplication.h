#ifndef CGLUT_APPLICATION_H_INCL
#define CGLUT_APPLICATION_H_INCL

#include "opengl.h"
#include "physics.h"
#include "glsl.h"
#include "CEventReceiver.h"
#include "CScene.h"
#include "CCameraFPS.h"
#include "CCameraFollower.h"
#include "CCar.h"
#include "CObjectCheckpointController.h"
#include "COverlayText.h"

struct SGLUTParameters
{
	SGLUTParameters() : WindowX(0), WindowY(0), WindowW(640), WindowH(480),
		WindowTitle(0), DoubleBuffering(true), FSAA(false), Car(0), SkyDome(0)
	{
	}

	int WindowX;
	int WindowY;
	int WindowW;
	int WindowH;
	const char* WindowTitle;
	bool DoubleBuffering;
	bool FSAA;
	char* Car;
	char* SkyDome;
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
	void generateCity();

	CCar* addCar(const char* propFile);
	void resetCar(CCar* car);

	bool keyDown(char key);
	bool keyUp(char key);
	bool keyPressed(char key);
	bool keyReleased(char key);

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
	void updateKeyStrokes();

	bool DoubleBuffering;

	CScene* Scene;
	CPhysicsWorld* Physics;
	cwc::glShaderManager* ShaderManager;
	btClock Clock;
	CObjectCheckpointController* CPController;

	CCameraFPS* CameraFPS;
	CCameraFollower* Camera;
	CCameraFollower* CameraBumper;
	CCar* Vehicle;
	std::vector<CCar*> Vehicles;
	//std::vector<CObject*> VehiclesClosest;
	std::vector<COverlayText*> Overlays;

	COverlayText* Overlay;
	CSpline* Spline;

	char* car;
	char* skyDome;

	bool KeyDown[256];
	bool KeyDownPrev[256];
	bool isPlaying;
};

#endif