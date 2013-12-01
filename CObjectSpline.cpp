#include "CObjectSpline.h"
#include "opengl.h"
#include "utils.h"

CObjectSpline::CObjectSpline(CSpline* spline,
	const std::vector<vec3>& stencil,
	int numberOfDivisions,
	CObject* parent,
	const vec3& pos,
	const vec3& rot) :
CObject(parent, pos, rot), Spline(spline)
{
	float t = 0.f;
	float end = (float) Spline->getNumberOfControlPoints();
	float dt = end / numberOfDivisions;

	uint w = (uint) stencil.size();

	mat4 basis(MT_NULL);
	Spline->getFrameBasis(t, basis);
	for (uint i=0; i<w; i++)
		Vertices.push_back(basis * stencil[i]);

	int idx = 0;
	while (t <= end)
	{
		t += dt;
		Spline->getFrameBasis(t, basis);
		for (uint i=0; i<w; i++)
			Vertices.push_back(basis * stencil[i]);

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

	// buffers generated, now push to opengl
	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);
}

void CObjectSpline::render()
{
	glPushMatrix();
	transform();

	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_TRIANGLES);
	for (uint i=0; i<Indices.size(); i++)
	{
		vec3& v = Vertices[Indices[i]];
		glNormal3f(0,1,0);
		glVertex3f(v.X, v.Y, v.Z);
	}
	glEnd();

	CObject::render();
	glPopMatrix();
}