#ifndef CSPLINE_H_INCL
#define CSPLINE_H_INCL

#include "vec3.h"
#include "mat4.h"
#include "SControlPoint.h"
#include <vector>

typedef std::vector<SControlPoint> ControlPointList;

class CSpline
{
public:
	void addControlPoint(const SControlPoint& p);

	int getNumberOfControlPoints() { return (int) Points.size(); }

	vec3 getPosition(float t) const;
	vec3 getDerivative(float t) const;
	vec3 getSecondDerivative(float t) const;

	vec3 getTangent(float t) const;
	vec3 getNormal(float t) const;
	vec3 getBinormal(float t) const;
	vec3 getUpVector(float t) const;

	float getCurvature(float t) const;

	float getClosestPoint(const vec3& v) const;

	// transformation from stencil to UDS frame
	void getFrameBasis(float t, mat4& result) const;

	void makeCardinal(float c);
	void makeCatmullRom();

private:
	int index(int i) const;
	int getLowIndex(float t) const;
	int getHighIndex(float t) const;

	ControlPointList Points;
};

#endif