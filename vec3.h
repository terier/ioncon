#ifndef VEC3_H_INCL
#define VEC3_H_INCL

#include <math.h> // sqrt

struct vec3
{
	float X, Y, Z;

	// Constructors
	vec3() : X(0), Y(0), Z(0) {}
	vec3(float x, float y, float z) : X(x), Y(y), Z(z) {}
	vec3(const vec3& v) : X(v.X), Y(v.Y), Z(v.Z) {}

	// Length squared
	float lensq() const
	{
		return X * X + Y * Y + Z * Z;
	}
	
	// Length
	float len() const
	{
		return sqrt(lensq());
	}

	// Dot product
	float dot(const vec3& v) const
	{
		return X * v.X + Y * v.Y + Z * v.Z;
	}

	// Cross product
	vec3 cross(const vec3& v) const
	{
		return vec3(
			Y * v.Z - Z * v.Y,
			Z * v.X - X * v.Z,
			X * v.Y - Y * v.X);
	}

	// Cosine of angle between this and v
	float cos(const vec3& v) const
	{
		return dot(v) / (len() * v.len());
	}
	
	// Normalized this, copy
	vec3 norm() const
	{
		return *this / len();
	}

	// Length of this projected onto v
	float projlen(const vec3& v) const
	{
		return dot(v) / v.len();
	}

	// Projection of this onto v, copy
	vec3 projvec(const vec3& v) const
	{
		return v * (dot(v) / v.lensq());
	}

	// Operators
	vec3 operator+(const vec3& v) const { return vec3(X + v.X, Y + v.Y, Z + v.Z); }
	vec3 operator-(const vec3& v) const { return vec3(X - v.X, Y - v.Y, Z - v.Z); }
	vec3 operator*(float a) const { return vec3(X * a, Y * a, Z * a); }
	vec3 operator/(float a) const { return vec3(X / a, Y / a, Z / a); }
	float operator*(const vec3& v) const { return dot(v); }

	vec3& operator=(const vec3& v)
	{
		X = v.X;
		Y = v.Y;
		Z = v.Z;
		return *this;
	}

	vec3& operator+=(const vec3& v)
	{
		X += v.X;
		Y += v.Y;
		Z += v.Z;
		return *this;
	}

	vec3& operator-=(const vec3& v)
	{
		X -= v.X;
		Y -= v.Y;
		Z -= v.Z;
		return *this;
	}

	vec3& operator*=(float a)
	{
		X *= a;
		Y *= a;
		Z *= a;
		return *this;
	}

	vec3& operator/=(float a)
	{
		X /= a;
		Y /= a;
		Z /= a;
		return *this;
	}
};

#endif