#ifndef COBJECT_MESH_H_INCL
#define COBJECT_MESH_H_INCL

#include "CObject.h"
#include "CMesh.h"

class CObjectMesh : public CObject
{
public:
	CObjectMesh(CMesh* Mesh, CObject* Parent = 0, const vec3& Pos = vec3(), const vec3& Rot = vec3()) :
		CObject(Parent, Pos, Rot), mesh(Mesh)
	{
	}

	void render();

private:
	CMesh* mesh;
};

#endif