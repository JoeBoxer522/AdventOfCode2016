#ifndef VECTOR2D_H
#define VECTOR2D_H

#include "Math.h"

class Vector2D {
public:
	int x;
	int y;
	Vector2D() :
		x(0), y(0)
	{}
	Vector2D(int inX, int inY) :
		x(inX), y(inY)
	{}
	Vector2D(const Vector2D& v) :
		x(v.x), y(v.y)
	{}

	Vector2D operator-(const Vector2D& v) const
	{
		return(Vector2D(x + v.x, y + v.y));
	}

	Vector2D operator+(const Vector2D& v) const
	{
		return(Vector2D(x + v.x, y + v.y));
	}

	Vector2D& operator+=(const Vector2D& v)
	{
		x += v.x;
		y += v.y;
		return(*this);
	}

	Vector2D operator*(const int magnitude) const
	{
		return(Vector2D(x * magnitude, y * magnitude));
	}

	Vector2D operator*(const float magnitude) const
	{
		return(Vector2D(int(float(x) * magnitude), int(float(y) * magnitude)));
	}

	bool operator==(const Vector2D& v) const
	{
		return(x == v.x && y == v.y);
	}

	void RotateByDegrees(double degrees)
	{
		RotateByRadians(ToRadians(degrees));
	}

	void RotateByRadians(double radians)
	{
		double newX = x * cos(radians) - y * sin(radians);
		double newY = x * sin(radians) + y * cos(radians);
		x = int(newX);
		y = int(newY);
	}

	float Length() const
	{
		return sqrt(float(sqr(x) + sqr(y)));
	}

	static float Dist(const Vector2D& v1, const Vector2D& v2)
	{
		return(v1 - v2).Length();
	}

	int Cross(const Vector2D& v1, const Vector2D& v2)
	{
		//U x V = Ux*Vy-Uy*Vx
		return((v1.x * v2.y) - (v1.y * v2.x));
	}
};

#endif // VECTOR2D_H