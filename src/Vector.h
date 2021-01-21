#ifndef VECTOR_H_
#define VECTOR_H_

class Vector {
public:
	Vector();
	Vector(double x, double y, double z);
	virtual ~Vector();

	Vector sub(Vector other);
	Vector add(Vector other);
	Vector mul(float fac);
	Vector cross(Vector other);
	Vector normalized();
	Vector rotateX(float a);
	Vector rotateY(float a);
	Vector rotateZ(float a);
	double length();
	double dot(Vector other);

	double x, y, z;
};

#endif /* VECTOR_H_ */
