#ifndef CSPLINE_H_INCL
#define CSPLINE_H_INCL

#include "vec3.h"
#include "SControlPoint.h"
#include <vector>

typedef std::vector<SControlPoint> ControlPointList;

class CSpline
{
public:
	void addControlPoint(const SControlPoint& p);

	vec3 getPosition(float t) const;
	vec3 getDerivative(float t) const;
	vec3 getSecondDerivative(float t) const;

	vec3 getTangent(float t) const;
	vec3 getNormal(float t) const;
	vec3 getBinormal(float t) const;

private:
	int getLowIndex(float t) const;
	int getHighIndex(float t) const;

	ControlPointList Points;
};

#endif