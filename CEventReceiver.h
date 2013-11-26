#ifndef CEVENT_RECEIVER_H_INCL
#define CEVENT_RECEIVER_H_INCL

class CEventReceiver
{
public:
	virtual ~CEventReceiver()
	{
	}

	virtual void displayFunc() {}
	virtual void idleFunc() {}
	virtual void mouseFunc(int button, int state, int x, int y) {}
	virtual void reshapeFunc(int w, int h) {}
	virtual void keyboardFunc(unsigned char c, int x, int y) {}
	virtual void keyboardUpFunc(unsigned char c, int x, int y) {}
	virtual void specialFunc(int c, int x, int y) {}
	virtual void motionFunc(int x, int y) {}
	virtual void passiveMotionFunc(int x, int y) {}
};

#endif