#ifndef COBJECT_MESH_H_INCL
#define COBJECT_MESH_H_INCL

#include "CObject.h"
#include "CMesh.h"
#include "glsl.h"
#include "CShaderCallback.h"

class CObjectMesh : public CObject
{
public:
	CObjectMesh(CMesh* mesh, CObject* parent = 0, const vec3& pos = vec3(), const vec3& rot = vec3()) :
		CObject(parent, pos, rot), Mesh(mesh), Shader(0), ShaderCallback(0)
	{
	}

	CMesh* getMesh() { return Mesh; }
	cwc::glShader* getShader() { return Shader; }
	CShaderCallback* getShaderCallback() { return ShaderCallback; }

	void setMesh(CMesh* mesh) { Mesh = mesh; }
	void setShader(cwc::glShader* shader) { Shader = shader; }
	void setShaderCallback(CShaderCallback* callback) { ShaderCallback = callback; }

	void render();

private:
	CMesh* Mesh;
	cwc::glShader* Shader;
	CShaderCallback* ShaderCallback;
};

#endif