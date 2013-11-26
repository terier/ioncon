#ifndef MAT4_H_INCL
#define MAT4_H_INCL

#include <math.h>
#include "vec3.h"

enum MATRIX_TYPE
{
	MT_NULL,
	MT_IDENTITY,
	MT_TRANSLATE,
	MT_SCALE,
	MT_ROTATEX,
	MT_ROTATEY,
	MT_ROTATEZ,
	MT_ROTATE,
	MT_PROJECTION
};

struct mat4
{
	mat4(MATRIX_TYPE mt = MT_IDENTITY, float* data = 0);
	mat4(const mat4& m);

	void setNull();
	void setIdentity();
	void setTranslate(const vec3& v);
	void setScale(const vec3& v);
	void setRotateX(float a);
	void setRotateY(float a);
	void setRotateZ(float a);
	void setRotate(const vec3& a);
	void setProjection(float d);

	mat4 operator*(const mat4& m);
	vec3 operator*(const vec3& v);

	float M[16];
};

#endif