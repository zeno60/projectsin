#ifndef COLOR_HEADER
#define COLOR_HEADER

class color
{
public:
	
	color(float r, float g, float b, float a):r(r),g(g),b(b),a(a) { }
	color():r(0.0f),g(0.0f),b(0.0f),a(1.0f){ }

	float r, g, b, a;
};

#endif