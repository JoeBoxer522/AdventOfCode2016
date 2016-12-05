#ifndef MATH_H
#define MATH_H

#define _USE_MATH_DEFINES
#include <math.h>

inline int sqr(int x)
{
	return x * x;
}

inline double sqr(double x)
{
	return x * x;
}

inline double ToRadians(double degrees)
{
	return degrees * (M_PI / 180);
}

inline double ToDegrees(double radians)
{
	return radians * (180.0 / M_PI);
}

// Inclusive clamp
template <typename T>
inline T Clamp(const T& n, const T& lower, const T& upper)
{
    return std::max(lower, std::min(n, upper));
}

#endif // MATH_H