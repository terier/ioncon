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

btVector3 createBulletVector(const vec3& v)
{
	return btVector3(v.X, v.Y, v.Z);
}

vec3 createIonconVector(const btVector3& v)
{
	return vec3(v.x(), v.y(), v.z());
}