#ifndef CMESH_H_INCL
#define CMESH_H_INCL

#include "CVertex.h"
#include <vector>
#include <string>
#include "tiny_obj_loader.h"

using namespace tinyobj;

typedef unsigned int uint;
typedef std::vector<shape_t> ShapeVector;
typedef std::vector<uint> UintVector;
typedef std::vector<float> FloatVector;

class CMesh
{
public:
	CMesh(const char* fname, const char* base = NULL);

	const ShapeVector& getShapes() const { return Shapes; }

private:
	ShapeVector Shapes;
};

#endif