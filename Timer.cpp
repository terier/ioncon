#include "CTimer.h"

CTimer::CTimer()
{
	StartTime = clock();
	Time = 0;
	IsRunning = false;
}

void CTimer::start()
{
	IsRunning = true;
	StartTime = clock();
}

void CTimer::stop()
{
	IsRunning = false;
	Time += clock() - StartTime;
}

void CTimer::reset()
{
	Time = 0;
	StartTime = clock();
}

clock_t CTimer::getTime()
{
	return Time + clock() - StartTime;
}

float CTimer::getTimeSeconds()
{
	if(IsRunning)
		return (Time + clock() - StartTime) / (float) CLOCKS_PER_SEC;
	else
		return (Time) / (float) CLOCKS_PER_SEC;
}