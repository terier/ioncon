#ifndef COBJECT_SPLINE_H_INCL
#define COBJECT_SPLINE_H_INCL

#include "CObject.h"
#include "CSpline.h"
#include <vector>

class CObjectSpline : public CObject
{
public:
	CObjectSpline(CSpline* spline, const std::vector<vec3>& stencil, int numberOfDivisions, float texscale = 1.f,
		CObject* parent = 0, const vec3& pos = vec3(), const vec3& rot = vec3());
	// stencil: x,y = position, z = x-texcoord

	void render();

private:
	CSpline* Spline;
	std::vector<vec3> Vertices;
	std::vector<vec3> Normals;
	std::vector<vec3> TexCoord;
	std::vector<uint> Indices;
	//GLuint VAO;
};

#endif