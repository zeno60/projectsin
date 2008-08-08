#ifndef MATRIX4_HEADER
#define MATRIX4_HEADER

#include "math.h"
#include "vector3d.h"

template<class T>
class matrix4
{
public:
	T m[16];
	
	T& operator()(const int row, const int col) { return m[row * 4 + col]; }
	const T& operator()(const int row, const int col) const { return m[row * 4 + col]; }

	T& operator[](int index) { return m[index]; }
	const T& operator[](int index) const { return m[index]; }

	matrix4<T> operator+(const matrix4<T>& other) const
	{
		matrix4<T> tmp;
		for(int i = 0; i < 16; i++)
		{
			tmp[i] = m[i]+other[i];
		}
		return tmp;
	}

	matrix4<T>& operator+=(const matrix4<T>& other)
	{
		for(int i = 0; i < 16; i++)
		{
			m[i] += other[i];
		}
		return *this;
	}

	matrix4<T> operator-(const matrix4<T>& other) const
	{
		matrix4<T> tmp;
		for(int i = 0; i < 16; i++)
		{
			tmp[i] = m[i]-other[i];
		}
		return tmp;
	}

	matrix4<T>& operator-=(const matrix4<T>& other)
	{
		for(int i = 0; i < 16; i++)
		{
			m[i] -= other[i];
		}
		return *this;
	}
	
	matrix4<T>& operator*=(const matrix4<T>& other)
	{
		//CMatrix4<T> temp ( *this );
		//setbyproduct ( temp, other );
		//return *this;
	}

	matrix4<T> operator*(const matrix4<T>& m2) const
	{
		// Testing purpose.. 
		if ( this->isIdentity() )
			return m2;
		if ( m2.isIdentity() )
			return *this;

		matrix4<T> m3;

		const T *m1 = m;

		m3[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
		m3[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
		m3[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
		m3[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

		m3[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
		m3[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
		m3[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
		m3[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

		m3[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
		m3[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
		m3[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
		m3[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

		m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
		m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
		m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
		m3[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
		return m3;
	}

	matrix4<T> operator*(const T& scalar) const
	{
		matrix4<T> tmp;
		for(int i = 0; i < 16; i++)
		{
			tmp[i] = m[i]*scalar;
		}
		return tmp;
	}

	matrix4<T>& operator*=(const T& scalar)
	{
		for(int i = 0; i < 16; i++)
		{
			m[i] *= scalar;
		}
		return *this;
	}
	
	matrix4<T>& buildPerspectiveFovLH(float fovy, float aspect, float zn, float zf)
	{
		makeIdentity();

		float delta = fovy/2.0f;
		float yScale = cos(delta)/sin(delta);
		float xScale = yScale/aspect;

		m[0] = xScale;
		m[5] = yScale;
		m[10] = zf/(zf-zn);
		m[11] = 1.0f;
		m[14] = -zn*zf/(zf-zn);
		m[15] = 0.0;

		return *this;
	}

	vector3df getTranslation()
	{
		return vector3df(m[12],m[13],m[14]);
	}

	void setTranslation(const vector3df& translation)
	{
		m[12] = translation.x;
		m[13] = translation.y;
		m[14] = translation.z;
	}
	
	vector3df getScale()
	{
		return vector3df(m[0],m[5],m[10]);
	}

	void setScale(const vector3df& scale)
	{
		m[0] = scale.x;
		m[5] = scale.y;
		m[10] = scale.z;
	}

	void setRotation(const vector3df& rotation)
	{
		double cr = cos( rotation.x );
		double sr = sin( rotation.x );
		double cp = cos( rotation.y );
		double sp = sin( rotation.y );
		double cy = cos( rotation.z );
		double sy = sin( rotation.z );

		m[0] = (T)( cp*cy );
		m[1] = (T)( cp*sy );
		m[2] = (T)( -sp );

		double srsp = sr*sp;
		double crsp = cr*sp;

		m[4] = (T)( srsp*cy-cr*sy );
		m[5] = (T)( srsp*sy+cr*cy );
		m[6] = (T)( sr*cp );

		m[8] = (T)( crsp*cy+sr*sy );
		m[9] = (T)( crsp*sy-sr*cy );
		m[10] = (T)( cr*cp );
	}
	
	void setRotationY(const float angle)
	{
		float c = cos(angle);
		float s = sin(angle);
		m[0] = (T)c;
		m[2] = (T)s;
		//m[2] = (T)(-0.5f * ( c + s) + 0.5f);

		m[8] = (T)-s;
		m[10] = (T)c;
		//m[6] = (T)(-0.5f * (-s + c) + 0.5f);
	}
	void setRotationAxis(const vector3df& axis, const float angle)
	{
		float s = sin(angle);
		float c = cos(angle);
		float t = 1 - c;
		
		vector3df ax = axis/sqrt(axis*axis);

		float x = ax.x;
		float y = ax.y;
		float z = ax.z;

		m[0] = t*x*x+c;
		m[1] = t*y*x+s*z;
		m[2] = t*z*x-s*y;
		m[4] = t*x*y-s*z;
		m[5] = t*y*y+c;
		m[6] = t*z*y+s*x;
		m[8] = t*x*z+s*y;
		m[9] = t*y*z-s*x;
		m[10] = t*z*z+c;
	}
	
	void transform(vector3df &v) const
	{
		float vector[3];

		vector[0] = v.x*m[0] + v.y*m[4] + v.z*m[8] + m[12];
		vector[1] = v.x*m[1] + v.y*m[5] + v.z*m[9] + m[13];
		vector[2] = v.x*m[2] + v.y*m[6] + v.z*m[10] + m[14];

		v.x = vector[0];
		v.y = vector[1];
		v.z = vector[2];
	}

	void makeIdentity()
	{
		int i = 0;
		for(int r = 0; r < 4; r++)
		{
			for(int c = 0; c < 4; c++)
			{
				(c==i) ? m[r*4+c]=(T)1 : m[r*4+c]=0;
			}
			i++;
		}
	}

	bool isIdentity() const
	{
		if( !math::equals(m[ 0],(T)1) || 
			!math::equals(m[ 5],(T)1) ||
			!math::equals(m[10],(T)1) || 
			!math::equals(m[15],(T)1) )
		{
			return false;
		}
		
		int i = 0;

		for(int r = 0; r < 4; r++)
		{
			for(int c = 0; c < 4; c++)
			{
				if((i!=c) && !math::equals(m[r*4+c],0))
				{
					return false;
				}
			}
			i++;
		}

		return true;
	}
};

typedef matrix4<float> matrixf;

#endif