#define _USE_MATH_DEFINES
#include "Quaternion.h"
#include <cmath>




Quaternion::Quaternion()
{
}
Quaternion::Quaternion(const Vector3f& xyz,float w):
	v(xyz),w(w)
{
}

Quaternion::~Quaternion()
{
}

void Quaternion::setConjugate(const Quaternion & q)
{
	w = q.w;
	v = q.v;
	v *= -1.0;
}

void Quaternion::set(const Vector3f & xyz, float w)
{
	v = xyz;
	this->w = w;
}

void Quaternion::setRotation(const Vector3f & axis, float angle)
{
	v = axis;
	v.normalize();

	v.x *= sin(M_PI*angle/360.0f);
	v.y *= sin(M_PI*angle /360.0f);
	v.z *= sin(M_PI*angle / 360.0f);
	w = cos(M_PI*angle / 360.0f);
	//v.debugWrite("q");
}

const Quaternion& Quaternion::operator*=(const Quaternion & q)
{
	Vector3f cross, wd_v, w_vd;
	wd_v = v;
	w_vd = q.v;
	wd_v *= q.w;
	w_vd *= w;
	//cross.setCross(v, q.v);
	cross = v.cross(q.v);
	w = w * q.w - v.dot(q.v);
	v = w_vd;
	v += wd_v;
	v += cross;

	return *this;
}
const Quaternion Quaternion::operator*(const Quaternion & q)
{
	Quaternion result;
	result = *this;
	result *= q;
	return std::move(result);
}

const Vector3f Quaternion::operator*(const Vector3f & v)
{

	return Vector3f();
}
