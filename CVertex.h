#ifndef CVERTEX_H_INCL
#define CVERTEX_H_INCL

#include "vec3.h"

struct CVertex
{
	CVertex(const vec3& Pos, const vec3& Nor = vec3(1,0,0), const vec3& Col = vec3(1,1,1), const vec3& Tex = vec3(1,1,0)) :
		pos(Pos), nor(Nor), col(Col), tex(Tex) {}

	vec3 pos;
	vec3 tex;
	vec3 col;
	vec3 nor;
};

#endif