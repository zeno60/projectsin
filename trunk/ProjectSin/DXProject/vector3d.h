#ifndef VECTOR3D_HEADER
#define VECTOR3D_HEADER

#include "d3dx10math.h"

template<class T>
class vector3d
{
public:
	T x, y, z;

	vector3d():x(0),y(0),z(0) { }
	vector3d(T tx, T ty, T tz):x(tx),y(ty),z(tz) { }
	vector3d(const vector3d &v):x(v.x),y(v.y),z(v.z) { }

	vector3d<T> &operator = (const vector3d<T> &a)
	{
		x = a.x;
		y = a.y;
		z = a.z;
		return *this;
	}

	vector3d<T>& operator+=(const vector3d<T>& a)
	{ 
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}
	
	vector3d<T> operator -(const vector3d<T> &a) const
	{
		return vector3d<T>(x - a.x, y - a.y, z - a.z);
	}

	vector3d<T> &operator - (const vector3d<T> &a)
	{
		return vector3d<T>(x - a.x, y - a.y, z - a.z);
	}
	
	//
	// Unary minus
	//
	vector3d<T> operator -() const
	{
		return vector3d<T>(-x, -y, -z);
	}

	vector3d<T> &operator *= (T a)
	{
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}
	
	vector3d<T> operator/(const vector3d<T>& other) const { return vector3d<T>(x / other.x, t / other.y, z / other.z);	}
	vector3d<T>& operator/=(const vector3d<T>& other)	{ x/=other.x; y/=other.y; z/=other.z; return *this; }
	vector3d<T> operator/(const T v) const { T i=(T)1.0/v; return vector3d<T>(x * i, y * i, z * i);	}
	vector3d<T>& operator/=(const T v) { T i=(T)1.0/v; x*=i; y*=i; z*=i; return *this; }

	vector3d<T> operator*(const T v) const 
	{ 
		return vector3d<T>(x * v, y * v, z * v);	
	}

	T operator * (const vector3d<T> &a) const
	{
		return dotProduct(a);
	}

	T dotProduct(const vector3d<T> &a) const
	{
		return x*a.x + y*a.y + z*a.z;
	}

	vector3d<T> crossProduct(const vector3d<T> &a) const
	{
		return vector3d<T>(
			y * a.z - z * a.y,
			z * a.x - x * a.z,
			x * a.y - y * a.x
		);
	}

	vector3d<T>& normalize()
	{
		float magSq = x*x + y*y + z*z;
		if(magSq > 0.0f)
		{
			float recipSqrt = 1.0f / sqrt(magSq);
			x *= recipSqrt;
			y *= recipSqrt;
			z *= recipSqrt;
		}
		return *this;
	}

	//
	// Requires D3DVECTOR3
	//
	vector3d<T> &operator = (const D3DXVECTOR3 &a)
	{
		x = a.x;
		y = a.y;
		z = a.z;
		return *this;
	}

	D3DXVECTOR3& toD3DXVECTOR3()
	{
		return D3DXVECTOR3(x, y, z);
	}
}; 

typedef vector3d<float> vector3df;

template<class S, class T> 
vector3d<T> operator*(const vector3d<S>& vector, const T scalar) 
{ 
	return vector*scalar; 
}

#endif
