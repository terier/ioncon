#ifndef COBJECT_MESH_H_INCL
#define COBJECT_MESH_H_INCL

#include "CObject.h"
#include "CMesh.h"

class CObjectMesh : public CObject
{
public:
	CObjectMesh(CMesh* mesh, CObject* parent = 0, const vec3& pos = vec3(), const vec3& rot = vec3()) :
		CObject(parent, pos, rot), Mesh(mesh), Texture(0)
	{
	}

	uint getTexture() const { return Texture; }
	void setTexture(uint tex) { Texture = tex; }

	void render();

private:
	CMesh* Mesh;
	uint Texture;
};

#endif