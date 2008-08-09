#ifndef MATH_H
#define MATH_H

namespace math
{
	const float ROUNDING_ERROR_32 = 0.000001f;
	const double ROUNDING_ERROR_64 = 0.00000001f;
	const float pi = 3.14159265f;

	inline bool equals(const float a, const float b, const float tolerance = ROUNDING_ERROR_32)
	{
		return (a + tolerance > b) && (a - tolerance < b);
	}
}

#endif