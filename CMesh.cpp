#include "CMesh.h"
#include <iostream>
#include <assert.h>

CMesh::CMesh()
{
}

CMesh::CMesh(const CMesh& other)
{
	Shapes = other.Shapes;
}

CMesh::CMesh(const char* fname, const char* base)
{
	std::string err = LoadObj(Shapes, fname, base);
	if (!err.empty())
		std::cout << err << "\n";
	else
		printf("Loaded %s\n", fname);
}

void CMesh::pushVector(FloatVector& where, const vec3& v)
{
	where.push_back(v.X);
	where.push_back(v.Y);
	where.push_back(v.Z);
}

void CMesh::generateFromSpline(CSpline* spline,
		const PointVector& stencil,
		int numberOfDivisions,
		float texscale)
{
	shape_t shape;
	UintVector& Indices = shape.mesh.indices;
	FloatVector& Vertices = shape.mesh.positions;
	FloatVector& Normals = shape.mesh.normals;
	FloatVector& TexCoord = shape.mesh.texcoords;

	float t = 0;
	float texcoord = 0;
	float end = (float) spline->getNumberOfControlPoints();
	float dt = end / numberOfDivisions;

	uint w = (uint) stencil.size();

	mat4 basis(MT_NULL);
	int idx = 0;
	bool firstIteration = true; // first iter create only verts, tris in later iters
	for (int subdiv=0; subdiv<=numberOfDivisions; subdiv++)
	{
		spline->getFrameBasis(t, basis);
		for (uint i=0; i<w; i++)
		{
			pushVector(Vertices, basis * vec3(stencil[i].X, stencil[i].Y, 0));
			TexCoord.push_back(stencil[i].Z);
			TexCoord.push_back(texcoord * texscale);
			if (i == 0) // starting stencil point
				pushVector(Normals, basis.mulnorm((stencil[i+1] - stencil[i]).cross(vec3(0,0,-1)).norm()));
			else if (i == w-1) // ending stencil point
				pushVector(Normals, basis.mulnorm((stencil[i] - stencil[i-1]).cross(vec3(0,0,-1)).norm()));
			else // middle stencil point
				pushVector(Normals, basis.mulnorm((stencil[i+1] - stencil[i-1]).cross(vec3(0,0,-1)).norm()));
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
}