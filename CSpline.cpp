#include "CSpline.h"

void CSpline::addControlPoint(const SControlPoint& p)
{
	Points.push_back(p);
}

int CSpline::getLowIndex(float t) const
{
	int size = Points.size();
	int i = (int)t % size;
	if (i < 0)
		i += size;
	return i;
}

int CSpline::getHighIndex(float t) const
{
	int i = getLowIndex(t);
	return (i + 1) % Points.size();
}

vec3 CSpline::getPosition(float t) const
{
	const SControlPoint& p0 = Points[getLowIndex(t)];
	const SControlPoint& p1 = Points[getHighIndex(t)];

	t = fmod(t, 1);
	float t2 = t*t;
	float t3 = t2*t;
	float t4 = t3*t;
	float t5 = t4*t;
	
	return
		p0.Pos * (1.f - 10.f*t3 + 15.f*t4 - 6.f*t5) +
		p1.Pos * (10.f*t3 - 15.f*t4 + 6.f*t5) +
		p0.Dir * (t - 6.f*t3 + 8.f*t4 - 3.f*t5) +
		p1.Dir * (-4.f*t3 + 7.f*t4 - 3.f*t5) +
		p0.Cur * (0.5f*t2 - 1.5f*t3 + 1.5f*t4 - 0.5f*t5) +
		p1.Cur * (0.5f*t3 - t4 + 0.5f*t5);
}

vec3 CSpline::getDerivative(float t) const
{
	const SControlPoint& p0 = Points[getLowIndex(t)];
	const SControlPoint& p1 = Points[getHighIndex(t)];

	t = fmod(t, 1);
	float t2 = t*t;
	float t3 = t2*t;
	float t4 = t3*t;
	
	return
		p0.Pos * (-30.f*t2 + 60.f*t3 - 30.f*t4) +
		p1.Pos * (30.f*t2 - 60.f*t3 + 30.f*t4) +
		p0.Dir * (1 - 18.f*t2 + 32.f*t3 - 15.f*t4) +
		p1.Dir * (-12.f*t2 + 28.f*t3 - 15.f*t4) +
		p0.Cur * (t - 4.5f*t2 + 6.f*t3 - 2.5f*t4) +
		p1.Cur * (1.5f*t2 - 4.f*t3 + 2.5f*t4);
}

vec3 CSpline::getSecondDerivative(float t) const
{
	const SControlPoint& p0 = Points[getLowIndex(t)];
	const SControlPoint& p1 = Points[getHighIndex(t)];

	t = fmod(t, 1);
	float t2 = t*t;
	float t3 = t2*t;
	
	return
		p0.Pos * (-60.f*t + 180.f*t2 - 120.f*t3) +
		p1.Pos * (60.f*t - 180.f*t2 + 120.f*t3) +
		p0.Dir * (-36.f*t + 96.f*t2 - 60.f*t3) +
		p1.Dir * (-24.f*t + 84.f*t2 - 60.f*t3) +
		p0.Cur * (1.f - 9.f*t + 18.f*t2 - 10.f*t3) +
		p1.Cur * (3.f*t - 12.f*t2 + 10.f*t3);
}

vec3 CSpline::getTangent(float t) const
{
	return getDerivative(t);
}

vec3 CSpline::getNormal(float t) const
{
	return getSecondDerivative(t);
}

vec3 CSpline::getBinormal(float t) const
{
	return getTangent(t).cross(getNormal(t));
}