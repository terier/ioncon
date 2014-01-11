#ifndef UTILS_H_INCL
#define UTILS_H_INCL

#include "vec3.h"
#include <btBulletDynamicsCommon.h> // btVector3

#define PI 3.14159265358979323846264f
#define PI2 1.5707963267948966192313f
#define RADTODEG 57.2957795130823209f
#define DEGTORAD 0.01745329251994329f

typedef unsigned int uint;

template <typename T>
T clamp(T v, T a, T b)
{
	(v < a ? a : v) > b ? b : v;
}

void printvec(const vec3& v);
void printfloat(float f);
float frand();
float frandm();

btVector3 createBulletVector(const vec3& v);
vec3 createIonconVector(const btVector3& v);

char* basename(const char* fname);

#endif