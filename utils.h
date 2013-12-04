#ifndef UTILS_H_INCL
#define UTILS_H_INCL

#include "vec3.h"
#include <btBulletDynamicsCommon.h> // btVector3

#define RADTODEG 57.2957795130823209f
#define DEGTORAD 0.01745329251994329f

template <typename T>
T clamp(T v, T a, T b)
{
	(v < a ? a : v) > b ? b : v;
}

void printvec(const vec3& v);
void printfloat(float f);

btVector3 createBulletVector(const vec3& v);
vec3 createIonconVector(const btVector3& v);

#endif