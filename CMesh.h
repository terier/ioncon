#ifndef CMESH_H_INCL
#define CMESH_H_INCL

#include <vector>
#include <string>
#include "tiny_obj_loader.h"
#include "CSpline.h"

using namespace tinyobj;

typedef unsigned int uint;
typedef std::vector<shape_t> ShapeVector;
typedef std::vector<uint> UintVector;
typedef std::vector<float> FloatVector;
typedef std::vector<vec3> PointVector;

class CMesh
{
public:
	CMesh();
	CMesh(const CMesh& other);
	CMesh(const char* fname, const char* base = NULL);

	const ShapeVector& getShapes() const { return Shapes; }

	void generateFromSpline(CSpline* spline,
		const PointVector& stencil,
		int numberOfDivisions,
		float texscale = 1.f);

private:
	void pushVector(FloatVector& where, const vec3& v);

	ShapeVector Shapes;
};

#endif