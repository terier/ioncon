#ifndef CCONTROL_POINT_H_INCL
#define CCONTROL_POINT_H_INCL

#include "vec3.h"

struct SControlPoint
{
	SControlPoint(const vec3& pos, const vec3& dir = vec3(), const vec3& cur = vec3(), const vec3& up = vec3(0,1,0)) :
		Pos(pos), Dir(dir), Cur(cur), Up(up)
	{
	}

	SControlPoint(const SControlPoint& p) :
		Pos(p.Pos), Dir(p.Dir), Cur(p.Cur), Up(p.Up)
	{
	}

	vec3 Pos;
	vec3 Dir;
	vec3 Cur;
	vec3 Up;
};

#endif