#include "CMesh.h"
#include <iostream>
#include <assert.h>

CMesh::CMesh()
{
}

CMesh::CMesh(const CMesh& other)
{
	Shapes = other.Shapes;
	generateVBO();
}

CMesh::CMesh(const char* fname, const char* base)
{
	std::string err = LoadObj(Shapes, fname, base);
	if (!err.empty())
		std::cout << err << "\n";
	else
		printf("Loaded %s\n", fname);
	generateVBO();
}

CMesh::CMesh(CSpline* spline,
	const PointVector& stencil,
	int numberOfDivisions,
	float texscale,
	const vec3& stencilscale)
{
	shape_t shape;
	shape.name = "Road Object";

	material_t& mat = shape.material;
	mat.emission[0] = mat.emission[1] = mat.emission[2] = 0;
	mat.ambient[0] = mat.ambient[1] = mat.ambient[2] = 0.3f;
	mat.diffuse[0] = mat.diffuse[1] = mat.diffuse[2] = 1;
	mat.specular[0] = mat.specular[1] = mat.specular[2] = 1;
	mat.shininess = 100;

	UintVector& Indices = shape.mesh.indices;
	FloatVector& Vertices = shape.mesh.positions;
	FloatVector& Normals = shape.mesh.normals;
	FloatVector& TexCoord = shape.mesh.texcoords;

	float t = 0;
	float texcoord = 0;
	float end = (float) spline->getNumberOfControlPoints();
	float dt = end / numberOfDivisions;

	uint w = (uint) stencil.size();
	PointVector Stencil(stencil);
	for (uint i=0; i<Stencil.size(); i++)
	{
		Stencil[i].X *= stencilscale.X;
		Stencil[i].Y *= stencilscale.Y;
	}

	mat4 basis(MT_NULL);
	int idx = 0;
	bool firstIteration = true; // first iter create only verts, tris in later iters
	for (int subdiv=0; subdiv<=numberOfDivisions; subdiv++)
	{
		spline->getFrameBasis(t, basis);
		for (uint i=0; i<w; i++)
		{
			pushVector(Vertices, basis * vec3(Stencil[i].X, Stencil[i].Y, 0));
			TexCoord.push_back(Stencil[i].Z);
			TexCoord.push_back(texcoord * texscale);
			if (i == 0) // starting stencil point
				pushVector(Normals, basis.mulnorm((Stencil[i+1] - Stencil[i]).cross(vec3(0,0,-1)).norm()));
			else if (i == w-1) // ending stencil point
				pushVector(Normals, basis.mulnorm((Stencil[i] - Stencil[i-1]).cross(vec3(0,0,-1)).norm()));
			else // middle stencil point
				pushVector(Normals, basis.mulnorm((Stencil[i+1] - Stencil[i-1]).cross(vec3(0,0,-1)).norm()));
		}

		if (!firstIteration) // first iter create only verts, tris in later iters
		{
			for (uint i=0; i<w-1; i++)
			{
				Indices.push_back(idx+i);
				Indices.push_back(idx+i+1);
				Indices.push_back(idx+i+w);
				Indices.push_back(idx+i+1);
				Indices.push_back(idx+i+w+1);
				Indices.push_back(idx+i+w);
			}
			idx += w;
		}
		firstIteration = false;
		t += dt;
		texcoord += spline->getDerivative(t).len();
	}

	Shapes.push_back(shape);
	generateVBO();
}

void CMesh::generateVBO()
{
	for (size_t i=0; i<Shapes.size(); i++)
	{
		vbo_t vbo;
		glGenBuffers(4, vbo.buffer);

		const mesh_t& m = Shapes[i].mesh;

		glBindBuffer(GL_ARRAY_BUFFER, vbo.buffer[VBO_VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, m.positions.size() * sizeof(float), m.positions.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vbo.buffer[VBO_NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, m.normals.size() * sizeof(float), m.normals.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vbo.buffer[VBO_TEXCOORD_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, m.texcoords.size() * sizeof(float), m.texcoords.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo.buffer[VBO_INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.indices.size() * sizeof(uint), m.indices.data(), GL_STATIC_DRAW);

		VBOs.push_back(vbo);
	}
	//Shapes.clear(); // free main memory
}

void CMesh::pushVector(FloatVector& where, const vec3& v)
{
	where.push_back(v.X);
	where.push_back(v.Y);
	where.push_back(v.Z);
}