#ifndef CCONTROL_POINT_H_INCL
#define CCONTROL_POINT_H_INCL

#include "vec3.h"

struct SControlPoint
{
	SControlPoint(const vec3& pos, const vec3& dir, const vec3& cur) :
		Pos(pos), Dir(dir), Cur(cur) {}
	SControlPoint(const SControlPoint& p) :
		Pos(p.Pos), Dir(p.Dir), Cur(p.Cur) {}

	vec3 Pos;
	vec3 Dir;
	vec3 Cur;
};

#endif