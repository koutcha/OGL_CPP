#pragma once
#include "Vector3.h"
class Quaternion
{
	friend class Matrix;
private:
	Vector3f v;
	float w;
public:
	Quaternion();
	Quaternion(const Vector3f & xyz, float w);
	~Quaternion();

	void setConjugate(const Quaternion & q);
	void set(const Vector3f& xyz, float w);
	//angle  is radian angle
	void setRotation(const Vector3f& axis, float angle);
	const Quaternion& operator *=(const Quaternion& q);
	const Quaternion operator *(const Quaternion& q);

	const Vector3f operator*(const Vector3f& v);

};

