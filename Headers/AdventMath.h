#ifndef MATH_H
#define MATH_H

#define _USE_MATH_DEFINES
#include <math.h>

int sqr(int x)
{
	return x * x;
}

double sqr(double x)
{
	return x * x;
}

double ToRadians(double degrees)
{
	return degrees * (M_PI / 180);
}

double ToDegrees(double radians)
{
	return radians * (180.0 / M_PI);
}

#endif // MATH_H