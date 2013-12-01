#ifndef COBJECT_SPLINE_H_INCL
#define COBJECT_SPLINE_H_INCL

#include "CObject.h"
#include "CSpline.h"
#include <vector>

class CObjectSpline : public CObject
{
public:
	CObjectSpline(CSpline* spline, const std::vector<vec3>& stencil, int numberOfDivisions, CObject* parent = 0, const vec3& pos = vec3(), const vec3& rot = vec3());

	//void createGeometry(const std::vector<vec3>& stencil, int numberOfDivisions);
	void render();

private:
	CSpline* Spline;
	std::vector<vec3> Vertices;
	std::vector<uint> Indices;
	//GLuint VAO;
};

#endif