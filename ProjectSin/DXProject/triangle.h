#ifndef TRIANGLE_HEADER
#define TRIANGLE_HEADER

template <class T>
struct triangle 
{

    T v[3]; // Array access useful in for loops
	
	triangle()
	{
		// nothing
	}

	triangle(T v0, T v1, T v2)
	{
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;
	}
};

#endif