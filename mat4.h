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

	mat4& makeNull();
	mat4& makeIdentity();
	mat4& makeTranslate(const vec3& v);
	mat4& makeScale(const vec3& v);
	mat4& makeRotateX(float a);
	mat4& makeRotateY(float a);
	mat4& makeRotateZ(float a);
	mat4& makeRotate(const vec3& a);
	mat4& makeProjection(float d);

	mat4& setTo(const mat4& m);

	mat4& setVector(int i1, int i2, int i3, const vec3& v);
	mat4& addVector(int i1, int i2, int i3, const vec3& v);
	mat4& mulVector(int i1, int i2, int i3, float a);
	vec3 getVector(int i1, int i2, int i3) const;
	mat4& setColumn(int i, const vec3& v);
	mat4& addColumn(int i, const vec3& v);
	mat4& mulColumn(int i, float a);
	vec3 getColumn(int i);
	mat4& setRow(int i, const vec3& v);
	mat4& addRow(int i, const vec3& v);
	mat4& mulRow(int i, float a);
	vec3 getRow(int i);

	mat4& makeTransposed();
	mat4 getTransposed() const;

	mat4 operator*(const mat4& m) const;
	vec3 operator*(const vec3& v) const;
	vec3 mulnorm(const vec3& v) const;

	mat4& operator=(const mat4& m);
	mat4& operator*=(const mat4& m);

	float M[16];
};

#endif