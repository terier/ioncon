#include "CSpline.h"
#include "utils.h"

void CSpline::addControlPoint(const SControlPoint& p)
{
	Points.push_back(p);
}

int CSpline::index(int i) const
{
	size_t size = Points.size();
	i %= size;
	return i < 0 ? i + size : i;
}

int CSpline::getLowIndex(float t) const
{
	return index((int)t);
}

int CSpline::getHighIndex(float t) const
{
	return index((int)(t+1));
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

vec3 CSpline::getUpVector(float t) const
{
	int i1 = index((int)t);
	int i2 = index(i1+1);
	int i3 = index(i2+1);
	int i0 = index(i1-1);

	const vec3& p0 = Points[i0].Up;
	const vec3& p1 = Points[i1].Up;
	const vec3& p2 = Points[i2].Up;
	const vec3& p3 = Points[i3].Up;

	t = fmod(t, 1);
	float t2 = t*t;
	float t3 = t2*t;

	return (
		p0 * (-t3 + 2.f*t2 -t) +
		p1 * (3.f*t3 - 5.f*t2 + 2.f) +
		p2 * (-3.f*t3 + 4.f*t2 + t) +
		p3 * (t3 - t2)
		) * 0.5f;
}

// transformation from stencil to UDS frame
void CSpline::getFrameBasis(float t, mat4& result) const
{
	vec3 P = getPosition(t);
	vec3 U = getUpVector(t);
	vec3 D = getTangent(t).norm();
	U -= U.projvec(D);
	U = U.norm();
	vec3 S = D.cross(U);

	result.setColumn(0, S);
	result.setColumn(1, U);
	result.setColumn(2, D);
	result.setColumn(3, P);
	result.setRow(3, vec3());
	result.M[15] = 1;
}

void CSpline::makeCardinal(float c)
{
	for (size_t i=0; i<Points.size(); i++)
		Points[i].Dir = (Points[index(i+1)].Pos - Points[index(i-1)].Pos) * (1 - c);
}

void CSpline::makeCatmullRom()
{
	makeCardinal(0);
}

float CSpline::getCurvature(float t) const
{
	vec3 d1 = getDerivative(t);
	vec3 d2 = getSecondDerivative(t);
	float c = d1.len();
	return (d1.cross(d2)).len() / (c * c * c);
}

float CSpline::getClosestPoint(const vec3& v) const
{
	float tend = (float) Points.size();

	float tmin = 0;
	float distmin = -1;
	float dist;
	for (float t=0; t<tend; t+=tend*0.02f)
	{
		dist = (getPosition(t) - v).lensq();
		if (dist < distmin || distmin < 0)
		{
			distmin = dist;
			tmin = t;
		}
	}

	float eps = 1e-2f;
	float t = tmin;
	float dist1, dist2, dist3;
	float d1, d2, grad, curv;
	float t1, t2, t3;

	// refine with quadratic minimization
	/*for (int i=0; i<4; i++)
	{
		t1 = t - eps;
		t2 = t;
		t3 = t + eps;
		dist1 = (getPosition(t1) - v).lensq();
		dist2 = (getPosition(t2) - v).lensq();
		dist3 = (getPosition(t3) - v).lensq();
		d1 = (dist2 - dist1) / eps;
		d2 = (dist3 - dist2) / eps;
		t = 0.5f * ((t2*t2-t3*t3)*dist1 + (t3*t3-t1*t1)*dist1 + (t1*t1-t2*t2)*dist3) /
			((t2-t3)*dist1 + (t3-t1)*dist2 + (t1-t2)*dist3);
	}*/

	// refine with newton's method
	for (int i=0; i<4; i++)
	{
		dist1 = (getPosition(t - eps) - v).lensq();
		dist2 = (getPosition(t) - v).lensq();
		dist3 = (getPosition(t + eps) - v).lensq();
		d1 = (dist2 - dist1) / eps;
		d2 = (dist3 - dist2) / eps;
		grad = (dist3 - dist1) / (2 * eps);
		curv = (d2 - d1) / eps;
		t -= grad / curv;
	}
	return t;
}