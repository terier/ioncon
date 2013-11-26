#ifndef CGLUT_INTERFACE_H_INCL
#define CGLUT_INTERFACE_H_INCL

#include "opengl.h"
#include "CEventReceiver.h"

class CGLUTInterface
{
public:
	static void connect(CEventReceiver* er)
	{
		printf("Connectiong application interface...\n");
		EventReceiver = er;

		glutDisplayFunc(displayFunc);
		glutIdleFunc(idleFunc);
		glutReshapeFunc(reshapeFunc);
		glutKeyboardFunc(keyboardFunc);
		glutKeyboardUpFunc(keyboardUpFunc);
		glutSpecialFunc(specialFunc);
		glutMouseFunc(mouseFunc);
		glutMotionFunc(motionFunc);
		glutPassiveMotionFunc(passiveMotionFunc);
	}

	static void disconnect()
	{
		EventReceiver = 0;
	}

	static void displayFunc()
	{
		if (EventReceiver)
			EventReceiver->displayFunc();
	}

	static void idleFunc()
	{
		if (EventReceiver)
			EventReceiver->idleFunc();
	}

	static void mouseFunc(int button, int state, int x, int y)
	{
		if (EventReceiver)
			EventReceiver->mouseFunc(button, state, x, y);
	}

	static void reshapeFunc(int w, int h)
	{
		if (EventReceiver)
			EventReceiver->reshapeFunc(w, h);
	}

	static void keyboardFunc(unsigned char c, int x, int y)
	{
		if (EventReceiver)
			EventReceiver->keyboardFunc(c, x, y);
	}

	static void keyboardUpFunc(unsigned char c, int x, int y)
	{
		if (EventReceiver)
			EventReceiver->keyboardUpFunc(c, x, y);
	}

	static void specialFunc(int c, int x, int y)
	{
		if (EventReceiver)
			EventReceiver->specialFunc(c, x, y);
	}

	static void motionFunc(int x, int y)
	{
		if (EventReceiver)
			EventReceiver->motionFunc(x, y);
	}

	static void passiveMotionFunc(int x, int y)
	{
		if (EventReceiver)
			EventReceiver->passiveMotionFunc(x, y);
	}

private:
	static CEventReceiver* EventReceiver;
};

// TODO yes i know, this is bad.
CEventReceiver* CGLUTInterface::EventReceiver = 0;

#endif