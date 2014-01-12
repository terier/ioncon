#include "utils.h"
#include <stdio.h>

void printvec(const vec3& v)
{
	printf("%5.2f, %5.2f, %5.2f\n", v.X, v.Y, v.Z);
}

void printfloat(float f)
{
	printf("%5.2f\n", f);
}

float frand()
{
	return rand() / (float) RAND_MAX;
}

float frandm()
{
	return (rand() / (float) (RAND_MAX / 2)) - 1;
}

btVector3 createBulletVector(const vec3& v)
{
	return btVector3(v.X, v.Y, v.Z);
}

vec3 createIonconVector(const btVector3& v)
{
	return vec3(v.x(), v.y(), v.z());
}

mat4 getOpenGLMatrixFromFrameBasis(const mat4& m)
{
	mat4 basis(m);
	basis.makeTransposed();
	basis.setRow(0, basis.getRow(0) * (-1));
	return basis;
}

char* basename(const char* fname)
{
	char* base = strdup(fname);
	char* s = strrchr(base, '/');
	s++;
	*s = '\0';
	return base;
}