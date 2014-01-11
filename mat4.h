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

	mat4& setNull();
	mat4& setIdentity();
	mat4& setTranslate(const vec3& v);
	mat4& setScale(const vec3& v);
	mat4& setRotateX(float a);
	mat4& setRotateY(float a);
	mat4& setRotateZ(float a);
	mat4& setRotate(const vec3& a);
	mat4& setProjection(float d);

	mat4& setTo(const mat4& m);

	mat4& transpose();
	mat4 getTransposed();

	mat4 operator*(const mat4& m) const;
	vec3 operator*(const vec3& v) const;
	vec3 mulnorm(const vec3& v) const;

	mat4& operator=(const mat4& m);
	mat4& operator*=(const mat4& m);

	float M[16];
};

#endif