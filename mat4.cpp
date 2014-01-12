#include "mat4.h"

mat4::mat4(MATRIX_TYPE mt, float* v)
{
	switch (mt)
	{
	case MT_NULL:
		makeNull();
		break;

	case MT_IDENTITY:
		makeIdentity();
		break;

	case MT_TRANSLATE:
		makeTranslate(vec3(v[0], v[1], v[2]));
		break;

	case MT_SCALE:
		makeScale(vec3(v[0], v[1], v[2]));
		break;

	case MT_ROTATEX:
		makeRotateX(v[0]);
		break;

	case MT_ROTATEY:
		makeRotateY(v[0]);
		break;

	case MT_ROTATEZ:
		makeRotateZ(v[0]);
		break;

	case MT_ROTATE:
		makeRotate(vec3(v[0], v[1], v[2]));
		break;

	case MT_PROJECTION:
		makeProjection(v[0]);
		break;
	}
}

mat4::mat4(const mat4& m)
{
	setTo(m);
}

mat4& mat4::makeNull()
{
	for (int i=0; i<16; i++)
		M[i] = 0;
	return *this;
}

mat4& mat4::makeIdentity()
{
	makeNull();
	M[0] = M[5] = M[10] = M[15] = 1;
	return *this;
}

mat4& mat4::makeTranslate(const vec3& v)
{
	makeIdentity();
	M[3] = v.X;
	M[7] = v.Y;
	M[11] = v.Z;
	return *this;
}

mat4& mat4::makeScale(const vec3& v)
{
	makeNull();
	M[0] = v.X;
	M[5] = v.Y;
	M[10] = v.Z;
	return *this;
}

mat4& mat4::makeRotateX(float a)
{
	float c = cos(a);
	float s = sin(a);

	makeIdentity();
	M[5] = M[10] = c;
	M[6] = -s;
	M[9] = s;

	return *this;
}

mat4& mat4::makeRotateY(float a)
{
	float c = cos(a);
	float s = sin(a);

	makeIdentity();
	M[0] = M[10] = c;
	M[8] = -s;
	M[2] = s;

	return *this;
}

mat4& mat4::makeRotateZ(float a)
{
	float c = cos(a);
	float s = sin(a);

	makeIdentity();
	M[0] = M[5] = c;
	M[1] = -s;
	M[4] = s;

	return *this;
}

mat4& mat4::makeRotate(const vec3& a)
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

mat4& mat4::makeProjection(float d)
{
	makeIdentity();
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

mat4 mat4::operator*(const mat4& m) const
{
	mat4 ret(MT_NULL);
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			for (int k=0; k<4; k++)
				ret.M[i+4*j] += M[k+4*j] * m.M[i+4*k];
	return ret;
}

vec3 mat4::operator*(const vec3& v) const
{
	float r[4] = {0};
	float m[4] = {v.X, v.Y, v.Z, 1};
	for (int i=0; i<4; i++)
		for (int k=0; k<4; k++)
			r[i] += M[k+4*i] * m[k];
	return vec3(r[0]/r[3], r[1]/r[3], r[2]/r[3]);
}

vec3 mat4::mulnorm(const vec3& v) const
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
	return *this;
}

mat4& mat4::operator*=(const mat4& m)
{
	setTo((*this) * m);
	return *this;
}

mat4& mat4::makeTransposed()
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

mat4 mat4::getTransposed() const
{
	mat4 ret(MT_NULL);
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			ret.M[i+4*j] = M[j+4*i];
	return ret;
}

mat4& mat4::setVector(int i1, int i2, int i3, const vec3& v)
{
	M[i1] = v.X;
	M[i2] = v.Y;
	M[i3] = v.Z;
	return *this;
}

mat4& mat4::addVector(int i1, int i2, int i3, const vec3& v)
{
	M[i1] += v.X;
	M[i2] += v.Y;
	M[i3] += v.Z;
	return *this;
}

mat4& mat4::mulVector(int i1, int i2, int i3, float a)
{
	M[i1] *= a;
	M[i2] *= a;
	M[i3] *= a;
	return *this;
}

vec3 mat4::getVector(int i1, int i2, int i3) const
{
	return vec3(M[i1], M[i2], M[i3]);
}

mat4& mat4::setColumn(int i, const vec3& v)
{
	return setVector(i, i + 4, i + 8, v);
}

mat4& mat4::addColumn(int i, const vec3& v)
{
	return addVector(i, i + 4, i + 8, v);
}

mat4& mat4::mulColumn(int i, float a)
{
	return mulVector(i, i + 4, i + 8, a);
}

vec3 mat4::getColumn(int i)
{
	return getVector(i, i + 4, i + 8);
}

mat4& mat4::setRow(int i, const vec3& v)
{
	int j = i * 4;
	return setVector(j, j + 1, j + 2, v);
}

mat4& mat4::addRow(int i, const vec3& v)
{
	int j = i * 4;
	return addVector(j, j + 1, j + 2, v);
}

mat4& mat4::mulRow(int i, float a)
{
	int j = i * 4;
	return mulVector(j, j + 1, j + 2, a);
}

vec3 mat4::getRow(int i)
{
	int j = i * 4;
	return getVector(j, j + 1, j + 2);
}