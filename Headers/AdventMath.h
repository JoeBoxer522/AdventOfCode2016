#ifndef ADVENT_MATH_H
#define ADVENT_MATH_H

#define _USE_MATH_DEFINES
#include <math.h>

template <typename T>
inline T sqr(T x)
{
	return(x * x);
}

inline double ToRadians(double degrees)
{
	return(degrees * (M_PI / 180));
}

inline double ToDegrees(double radians)
{
	return(radians * (180.0 / M_PI));
}

inline int ToInt(char c)
{
    return(c - '0');
}

// Inclusive clamp
template <typename T>
inline T Clamp(const T& n, const T& lower, const T& upper)
{
    return std::max(lower, std::min(n, upper));
}

#endif // ADVENT_MATH_H