#ifndef COBJECT_SHAPES_H_INCL
#define COBJECT_SHAPES_H_INCL

#include "CObject.h"

// Plane -----------------------------------------------------------------
class CObjectPlane : public CObject
{
public:
	CObjectPlane(float size = 10.f, int subdivide = 1) :
	  Size(size), Subdivide(subdivide)
	{
	}

	float getSize() const { return Size; }
	int getSubdivide() const { return Subdivide; }

	void setSize(float size) { Size = size; }
	void setSubdivide(int subdivide) { Subdivide = subdivide; }

	void render()
	{
		glPushMatrix();
		transform();
		glBindTexture(GL_TEXTURE_2D, getTexture());
		glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		float d = 1.f / Subdivide;
		float s = Size * d;
		for (int i=0; i<Subdivide; i++)
		{
			for (int j=0; j<Subdivide; j++)
			{
				glTexCoord2f(i*d, (j+1)*d);
				glVertex3f(i*s, 0, (j+1)*s);
				glTexCoord2f((i+1)*d, (j+1)*d);
				glVertex3f((i+1)*s, 0, (j+1)*s);
				glTexCoord2f((i+1)*d, j*d);
				glVertex3f((i+1)*s, 0, j*s);
				glTexCoord2f(i*d, j*d);
				glVertex3f(i*s, 0, j*s);
			}
		}
		glEnd();
		CObject::render();
		glPopMatrix();
	}

private:
	float Size;
	int Subdivide;
};

// Sphere ----------------------------------------------------------------
class CObjectSphere : public CObject
{
public:
	CObjectSphere(float radius = 5.f, int slices = 32, int stacks = 16) :
	  Radius(radius), Slices(slices), Stacks(stacks)
	{
		Sphere = gluNewQuadric();
		gluQuadricDrawStyle(Sphere, GLU_FILL);
		gluQuadricTexture(Sphere, GL_TRUE);
		gluQuadricNormals(Sphere, GLU_SMOOTH);
	}

	~CObjectSphere()
	{
		gluDeleteQuadric(Sphere);
	}

	float getRadius() const { return Radius; }
	int getSlices() const { return Slices; }
	int getStacks() const { return Stacks; }

	void setRadius(float radius) { Radius = radius; }
	void setSlices(int slices) { Slices = slices; }
	void setStacks(int stacks) { Stacks = stacks; }

	void render()
	{
		glPushMatrix();
		transform();
		glBindTexture(GL_TEXTURE_2D, getTexture());
		//glutSolidSphere(Radius, Slices, Stacks);
		gluSphere(Sphere, Radius, Slices, Stacks);
		CObject::render();
		glPopMatrix();
	}

private:
	GLUquadric* Sphere;
	float Radius;
	int Slices;
	int Stacks;
};

// Cube ------------------------------------------------------------------
class CObjectCube : public CObject
{
public:
	CObjectCube(float size = 5.f) :
	  Size(size)
	{
	}

	float getSize() const { return Size; }
	void setSize(float size) { Size = size; }

	void render()
	{
		glPushMatrix();
		transform();
		glutSolidCube(Size);
		CObject::render();
		glPopMatrix();
	}

private:
	float Size;
};

// Cone ------------------------------------------------------------------
class CObjectCone : public CObject
{
public:
	CObjectCone(float base = 1.f, float height = 1.f, int slices = 16, int stacks = 32) :
	 Base(base), Height(height), Slices(slices), Stacks(stacks)
	{
	}

	float getBase() const { return Base; }
	float getHeight() const { return Height; }
	int getSlices() const { return Slices; }
	int getStacks() const { return Stacks; }

	void setBase(float base) { Base = base; }
	void setHeight(float height) { Height = height; }
	void setSlices(int slices) { Slices = slices; }
	void setStacks(int stacks) { Stacks = stacks; }

	void render()
	{
		glPushMatrix();
		transform();
		glutSolidCone(Base, Height, Slices, Stacks);
		CObject::render();
		glPopMatrix();
	}

private:
	float Base;
	float Height;
	int Slices;
	int Stacks;
};

// Dodecahedron ----------------------------------------------------------
class CObjectDodecahedron : public CObject
{
public:
	CObjectDodecahedron(float size = 1.f) :
	  Size(size)
	{
	}

	float getSize() const { return Size; }
	void setSize(float size) { Size = size; }

	void render()
	{
		glPushMatrix();
		transform();
		glScalef(Size, Size, Size);
		glutSolidDodecahedron();
		CObject::render();
		glPopMatrix();
	}

private:
	float Size;
};

// Icosahedron -----------------------------------------------------------
class CObjectIcosahedron : public CObject
{
public:
	CObjectIcosahedron(float size = 1.f) :
	  Size(size)
	{
	}

	float getSize() const { return Size; }
	void setSize(float size) { Size = size; }

	void render()
	{
		glPushMatrix();
		transform();
		glScalef(Size, Size, Size);
		glutSolidIcosahedron();
		CObject::render();
		glPopMatrix();
	}

private:
	float Size;
};

// Octahedron ------------------------------------------------------------
class CObjectOctahedron : public CObject
{
public:
	CObjectOctahedron(float size = 1.f) :
	  Size(size)
	{
	}

	float getSize() const { return Size; }
	void setSize(float size) { Size = size; }

	void render()
	{
		glPushMatrix();
		transform();
		glScalef(Size, Size, Size);
		glutSolidOctahedron();
		CObject::render();
		glPopMatrix();
	}

private:
	float Size;
};

// Tetrahedron -----------------------------------------------------------
class CObjectTetrahedron : public CObject
{
public:
	CObjectTetrahedron(float size = 1.f) :
	  Size(size)
	{
	}

	float getSize() const { return Size; }
	void setSize(float size) { Size = size; }

	void render()
	{
		glPushMatrix();
		transform();
		glScalef(Size, Size, Size);
		glutSolidTetrahedron();
		CObject::render();
		glPopMatrix();
	}

private:
	float Size;
};

// Teapot ----------------------------------------------------------------
class CObjectTeapot : public CObject
{
public:
	CObjectTeapot(float size = 1.f) :
	  Size(size)
	{
	}

	float getSize() const { return Size; }
	void setSize(float size) { Size = size; }

	void render()
	{
		glPushMatrix();
		transform();
		glutSolidTeapot(Size);
		CObject::render();
		glPopMatrix();
	}

private:
	float Size;
};

// Torus -----------------------------------------------------------------
class CObjectTorus : public CObject
{
public:
	CObjectTorus(float innerRadius = 1.f, float outerRadius = 2.f, int sides = 16, int rings = 32) :
	 InnerRadius(innerRadius), OuterRadius(outerRadius), Sides(sides), Rings(rings)
	{
	}

	float getInnerRadius() const { return InnerRadius; }
	float getOuterRadius() const { return OuterRadius; }
	int getSides() const { return Sides; }
	int getRings() const { return Rings; }

	void setInnerRadius(float innerRadius) { InnerRadius = innerRadius; }
	void setOuterRadius(float outerRadius) { OuterRadius = outerRadius; }
	void setSides(int sides) { Sides = sides; }
	void setRings(int rings) { Rings = rings; }

	void render()
	{
		glPushMatrix();
		transform();
		glutSolidTorus(InnerRadius, OuterRadius, Sides, Rings);
		CObject::render();
		glPopMatrix();
	}

private:
	float InnerRadius;
	float OuterRadius;
	int Sides;
	int Rings;
};

#endif