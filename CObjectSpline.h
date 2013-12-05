#ifndef COBJECT_SPLINE_H_INCL
#define COBJECT_SPLINE_H_INCL

#include "CObject.h"
#include "CSpline.h"
#include <vector>

typedef std::vector<vec3> PointVector;

class CObjectSpline : public CObject
{
public:
	CObjectSpline(CSpline* spline, const PointVector& stencil, int numberOfDivisions, float texscale = 1.f,
		CObject* parent = 0, const vec3& pos = vec3(), const vec3& rot = vec3());
	// stencil: x,y = position, z = x-texcoord

	void render();

	const PointVector& getVertexBuffer() const { return Vertices; }
	const std::vector<uint>& getIndexBuffer() const { return Indices; }

private:
	CSpline* Spline;
	PointVector Vertices;
	PointVector Normals;
	PointVector TexCoord;
	std::vector<uint> Indices;
	//GLuint VAO;
};

#endif