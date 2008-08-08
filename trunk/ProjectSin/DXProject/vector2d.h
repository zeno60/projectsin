#ifndef VECTOR2D_HEADER
#define VECTOR2D_HEADER

template<class T>
class vector2d
{
public:
	T x, y;

	vector2d():x(0),y(0) { }
	vector2d(T tx, T ty):x(tx),y(ty) { }
	vector2d(const vector2d &v):x(v.x),y(v.y) { }

	T length() const { return (T)sqrt(x*x + y*y); }
};

typedef vector2d<float> vector2df;

#endif