#ifndef QUATERNION_H
#define QUATERNION_H

#include "math.h"
#include "vector3d.h"
#include "matrix4.h"

class quaternion
{
public:
	quaternion():x(0.0f),y(0.0f),z(0.0f),w(1.0f) { }
	quaternion(float x, float y, float z, float w):x(x),y(y),z(z),w(w) { }
	quaternion(const vector3df &v) { set(v.x, v.y, v.z); }
	quaternion(const matrixf &m) { (*this) = m; }
	~quaternion() { }
	
	quaternion operator*(const quaternion &other) const
	{
		quaternion tmp;

		tmp.w = (other.w * w) - (other.x * x) - (other.y * y) - (other.z * z);
		tmp.x = (other.w * x) + (other.x * w) + (other.y * z) - (other.z * y);
		tmp.y = (other.w * y) + (other.y * w) + (other.z * x) - (other.x * z);
		tmp.z = (other.w * z) + (other.z * w) + (other.x * y) - (other.y * x);

		return tmp;
	}

	void set(float x, float y, float z)
	{
		float angle;

		angle = x * 0.5;
		float sr = (float)sin(angle);
		float cr = (float)cos(angle);

		angle = y * 0.5;
		float sp = (float)sin(angle);
		float cp = (float)cos(angle);

		angle = z * 0.5;
		float sy = (float)sin(angle);
		float cy = (float)cos(angle);

		float cpcy = cp * cy;
		float spcy = sp * cy;
		float cpsy = cp * sy;
		float spsy = sp * sy;

		this->x = (float)(sr * cpcy - cr * spsy);
		this->y = (float)(cr * spcy + sr * cpsy);
		this->z = (float)(cr * cpsy - sr * spcy);
		this->w = (float)(cr * cpcy + sr * spsy);

		normalize();
	}

	quaternion& normalize()
	{
		float n = x*x + y*y + z*z + w*w;
		if(n == 1) return *this;

		n = 1.0f / sqrtf(n);

		x *= n;
		y *= n;
		z *= n;
		w *= n;

		return *this;
	}

	void makeIdentity()
	{
		w = 1.0f;
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	
	matrixf getMatrix() const
	{
		matrixf m;

		m(0,0) = 1.0f - 2.0f*y*y - 2.0f*z*z;
		m(1,0) = 2.0f*x*y + 2.0f*z*w;
		m(2,0) = 2.0f*x*z - 2.0f*y*w;
		m(3,0) = 0.0f;

		m(0,1) = 2.0f*x*y - 2.0f*z*w;
		m(1,1) = 1.0f - 2.0f*x*x - 2.0f*z*z;
		m(2,1) = 2.0f*z*y + 2.0f*x*w;
		m(3,1) = 0.0f;

		m(0,2) = 2.0f*x*z + 2.0f*y*w;
		m(1,2) = 2.0f*z*y - 2.0f*x*w;
		m(2,2) = 1.0f - 2.0f*x*x - 2.0f*y*y;
		m(3,2) = 0.0f;

		m(0,3) = 0.0f;
		m(1,3) = 0.0f;
		m(2,3) = 0.0f;
		m(3,3) = 1.0f;

		return m;
	}

	float x, y, z, w;
};

#endif