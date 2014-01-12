#ifndef CTIMER_H_INCL
#define CTIMER_H_INCL

#include <time.h>

class CTimer
{
public:
	CTimer();
	clock_t getTime();
	float getTimeSeconds();
	void start();
	void stop();
	void reset();
private:
	clock_t Time;
	clock_t StartTime;
	bool IsRunning;
};

#endif