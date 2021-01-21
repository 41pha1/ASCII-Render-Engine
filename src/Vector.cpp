#include "Vector.h"

#include <cmath>


Vector::Vector(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector::~Vector()
{

}

Vector Vector::sub(Vector other)
{
	return Vector(x-other.x, y-other.y, z-other.z);
}

Vector Vector::add(Vector other)
{
	return Vector(x+other.x, y+other.y, z+other.z);
}

Vector Vector::mul(float fac)
{
	return Vector(x*fac, y*fac, z*fac);
}

double Vector::length()
{
	return sqrt(x*x+y*y+z*z);
}

Vector Vector::cross(Vector other)
{
	 double crossx = y * other.z - z * other.y;
	 double crossy = z * other.x - x * other.z;
	 double crossz = x * other.y - y * other.x;

	 return Vector(crossx,crossy, crossz);
}

Vector Vector::normalized()
{
	double length = this->length();
	return Vector(x/length, y/length, z*length);
}

Vector Vector::rotateZ(float a)
{
	double newx = x * cos(a) - y * sin(a);
	double newy = x * sin(a) + y * cos(a);
	double newz = z;

	return Vector(newx, newy, newz);
}

Vector Vector::rotateY(float a)
{
	double newx = x * cos(a) + z * sin(a);
	double newy = y;
	double newz = -x* sin(a) + z * cos(a);

	return Vector(newx, newy, newz);
}

Vector Vector::rotateX(float a)
{
	double newx = x;
	double newy = y * cos(a) - z * sin(a);
	double newz = y * sin(a) + z * cos(a);

	return Vector(newx, newy, newz);
}

double Vector::dot(Vector other)
{
	return x*other.x+y*other.y+z*other.z;
}

