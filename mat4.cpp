#include "mat4.h"

mat4::mat4(MATRIX_TYPE mt, float* v)
{
	switch (mt)
	{
	case MT_NULL:
		setNull();
		break;

	case MT_IDENTITY:
		setIdentity();
		break;

	case MT_TRANSLATE:
		setTranslate(vec3(v[0], v[1], v[2]));
		break;

	case MT_SCALE:
		setScale(vec3(v[0], v[1], v[2]));
		break;

	case MT_ROTATEX:
		setRotateX(v[0]);
		break;

	case MT_ROTATEY:
		setRotateY(v[0]);
		break;

	case MT_ROTATEZ:
		setRotateZ(v[0]);
		break;

	case MT_ROTATE:
		setRotate(vec3(v[0], v[1], v[2]));
		break;

	case MT_PROJECTION:
		setProjection(v[0]);
		break;
	}
}

mat4::mat4(const mat4& m)
{
	setTo(m);
}

mat4& mat4::setNull()
{
	for (int i=0; i<16; i++)
		M[i] = 0;
	return *this;
}

mat4& mat4::setIdentity()
{
	setNull();
	M[0] = M[5] = M[10] = M[15] = 1;
	return *this;
}

mat4& mat4::setTranslate(const vec3& v)
{
	setIdentity();
	M[3] = v.X;
	M[7] = v.Y;
	M[11] = v.Z;
	return *this;
}

mat4& mat4::setScale(const vec3& v)
{
	setNull();
	M[0] = v.X;
	M[5] = v.Y;
	M[10] = v.Z;
	return *this;
}

mat4& mat4::setRotateX(float a)
{
	float c = cos(a);
	float s = sin(a);

	setIdentity();
	M[5] = M[10] = c;
	M[6] = -s;
	M[9] = s;

	return *this;
}

mat4& mat4::setRotateY(float a)
{
	float c = cos(a);
	float s = sin(a);

	setIdentity();
	M[0] = M[10] = c;
	M[8] = -s;
	M[2] = s;

	return *this;
}

mat4& mat4::setRotateZ(float a)
{
	float c = cos(a);
	float s = sin(a);

	setIdentity();
	M[0] = M[5] = c;
	M[1] = -s;
	M[4] = s;

	return *this;
}

mat4& mat4::setRotate(const vec3& a)
{
	float cx = cos(a.X);
	float sx = sin(a.X);
	float cy = cos(a.Y);
	float sy = sin(a.Y);
	float cz = cos(a.Z);
	float sz = sin(a.Z);

	M[0] = cy * cz;
	M[1] = cz * sx * sy - cx * sz;
	M[2] = cx * cz * sy + sx * sz;
	M[3] = 0;
	M[4] = cy * sz;
	M[5] = cx * cz + sx * sy * sz;
	M[6] = cx * sy * sz - cz * sx;
	M[7] = 0;
	M[8] = -sy;
	M[9] = cy * sx;
	M[10] = cx * cy;
	M[11] = M[12] = M[13] = M[14] = 0;
	M[15] = 1;

	return *this;
}

mat4& mat4::setProjection(float d)
{
	setIdentity();
	M[15] = 0;
	M[14] = 1.f / d;
	return *this;
}

mat4& mat4::setTo(const mat4& m)
{
	for (int i=0; i<16; i++)
		M[i] = m.M[i];
	return *this;
}

mat4 mat4::operator*(const mat4& m)
{
	mat4 ret(MT_NULL);
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			for (int k=0; k<4; k++)
				ret.M[i+4*j] += M[k+4*j] * m.M[i+4*k];
	return ret;
}

vec3 mat4::operator*(const vec3& v)
{
	float r[4] = {0};
	float m[4] = {v.X, v.Y, v.Z, 1};
	for (int i=0; i<4; i++)
		for (int k=0; k<4; k++)
			r[i] += M[k+4*i] * m[k];
	return vec3(r[0]/r[3], r[1]/r[3], r[2]/r[3]);
}

vec3 mat4::mulnorm(const vec3& v)
{
	float r[4] = {0};
	float m[4] = {v.X, v.Y, v.Z, 0};
	for (int i=0; i<4; i++)
		for (int k=0; k<4; k++)
			r[i] += M[k+4*i] * m[k];
	return vec3(r[0], r[1], r[2]);
}

mat4& mat4::operator=(const mat4& m)
{
	setTo(m);
}

mat4& mat4::operator*=(const mat4& m)
{
	setTo((*this) * m);
	return *this;
}

mat4& mat4::transpose()
{
	float T;
	for (int i=0; i<3; i++)
	{
		for (int j=i+1; j<4; j++)
		{
			T = M[i+4*j];
			M[i+4*j] = M[j+4*i];
			M[j+4*i] = T;
		}
	}
	return *this;
}

mat4 mat4::getTransposed()
{
	mat4 ret(MT_NULL);
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			ret.M[i+4*j] = M[j+4*i];
	return ret;
}