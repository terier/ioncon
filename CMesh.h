#ifndef CMESH_H_INCL
#define CMESH_H_INCL

#include <vector>
#include <string>
#include "tiny_obj_loader.h"
#include "CSpline.h"
#include "vbo.h"
#include "opengl.h"

using namespace tinyobj;

typedef unsigned int uint;
typedef std::vector<shape_t> ShapeVector;
typedef std::vector<uint> UintVector;
typedef std::vector<float> FloatVector;
typedef std::vector<vec3> PointVector;
typedef std::vector<vbo_t> VBOVector;

class CMesh
{
public:
	CMesh();
	CMesh(const CMesh& other);
	CMesh(const char* fname);
	CMesh(CSpline* spline,
		const PointVector& stencil,
		int numberOfDivisions,
		float texscale = 1.f,
		const vec3& stencilscale = vec3(1,1,0));

	const ShapeVector& getShapes() const { return Shapes; }
	const VBOVector& getVBOs() const { return VBOs; }

private:
	void generateVBO();
	void pushVector(FloatVector& where, const vec3& v);

	ShapeVector Shapes;
	VBOVector VBOs;
};

#endif