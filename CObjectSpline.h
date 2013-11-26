#ifndef COBJECT_SPLINE_H_INCL
#define COBJECT_SPLINE_H_INCL

#include "CObject.h"
#include "CSpline.h"

class CObjectSpline : public CObject
{
public:
	CObjectSpline(CSpline* Spline, CObject* Parent = 0, const vec3& Pos = vec3(), const vec3& Rot = vec3()) :
		CObject(Parent, Pos, Rot)
	{
	}

	void createGeometry(CSpline* spline);
	void render();

private:
	// TODO
	// list of vertices for linelist/linestrip
	// or display list index?
};

#endif