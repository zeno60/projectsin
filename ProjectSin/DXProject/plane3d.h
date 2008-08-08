#ifndef PLANE3D_HEADER
#define PLANE3D_HEADER

#include "vector3d.h"

template<class T>
class plane3d
{
public:
	vector3d<T> normal;
	T distance;

	plane3d():normal(0,1,0) { calculateD(vector3d<T>(0,0,0)); }
	plane3d(const vector3d<T> &normal, const vector3d<T> &point):normal(normal) { calculateD(point); }
	plane3d(const vector3d<T> &p1, const vector3d<T> &p2, const vector3d<T> &p3) { setPlane(p1, p2, p3); }

	void setPlane(const vector3d<T> &p1, const vector3d<T> &p2, const vector3d<T> &p3)
	{
		normal = (p2 - p1).crossProduct(p3 - p2);
		normal.normalize();

		calculateD(p1);
	}

	void calculateD(const vector3d<T>& point)
	{
		distance = - point * normal;
	}
};

typedef plane3d<float> plane3df;

#endif