#include "CObjectSpline.h"
#include "opengl.h"
#include "utils.h"

CObjectSpline::CObjectSpline(CSpline* spline,
	const std::vector<vec3>& stencil,
	int numberOfDivisions,
	float texscale,
	CObject* parent,
	const vec3& pos,
	const vec3& rot) :
CObject(parent, pos, rot), Spline(spline)
{
	float t = 0;
	float texcoord = 0;
	float end = (float) Spline->getNumberOfControlPoints();
	float dt = end / numberOfDivisions;

	uint w = (uint) stencil.size();

	mat4 basis(MT_NULL);
	int idx = 0;
	bool firstIteration = true; // first iter create only verts, tris in later iters
	for (int subdiv=0; subdiv<=numberOfDivisions; subdiv++)
	{
		Spline->getFrameBasis(t, basis);
		for (uint i=0; i<w; i++)
		{
			Vertices.push_back(basis * vec3(stencil[i].X, stencil[i].Y, 0));
			TexCoord.push_back(vec3(stencil[i].Z, texcoord * texscale, 0));
			if (i == 0) // starting stencil point
			{
				Normals.push_back(basis.mulnorm(
					(stencil[i+1] - stencil[i]).cross(vec3(0,0,-1)).norm()
				));
			}
			else if (i == w-1) // ending stencil point
			{
				Normals.push_back(basis.mulnorm(
					(stencil[i] - stencil[i-1]).cross(vec3(0,0,-1)).norm()
				));
			}
			else // middle stencil point
			{
				Normals.push_back(basis.mulnorm(
					(stencil[i+1] - stencil[i-1]).cross(vec3(0,0,-1)).norm()
				));
			}
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
		texcoord += Spline->getDerivative(t).len();
	}

	// buffers generated, now push to opengl
	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);
}

void CObjectSpline::render()
{
	glPushMatrix();
	transform();

	glBindTexture(GL_TEXTURE_2D, getTexture());
	glBegin(GL_TRIANGLES);
	for (uint i=0; i<Indices.size(); i++)
	{
		vec3& v = Vertices[Indices[i]];
		vec3& n = Normals[Indices[i]];
		vec3& t = TexCoord[Indices[i]];
		glTexCoord2f(t.X, t.Y);
		glNormal3f(n.X, n.Y, n.Z);
		glVertex3f(v.X, v.Y, v.Z);
	}
	glEnd();

	CObject::render();
	glPopMatrix();
}