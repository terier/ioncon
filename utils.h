#ifndef UTILS_H_INCL
#define UTILS_H_INCL

#include "opengl.h"
#include "stb_image.h"
#include "vec3.h"

#define RADTODEG 57.2957795130823209f
#define DEGTORAD 0.01745329251994329f

template <typename T>
T clamp(T v, T a, T b)
{
	(v < a ? a : v) > b ? b : v;
}

void printvec(const vec3& v);
void printfloat(float f);

unsigned int loadTexture(const char* fname);

#endif