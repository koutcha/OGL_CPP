#pragma once

#include "Vector3.h"
#include "RagidObject.h"
#include "object.h"
#include <array>

namespace MyMath 
{

	float SquaringlengthAABBToPoint(const Vector3f& boxCenter, const Vector3f& boxSize, const Vector3f& point);

	Vector3f Vector3AABBToPoint(const Vector3f & boxCenter, const Vector3f & boxSize, const Vector3f & point);

	void checkWall(float wallHalfWidth, float wallHalfDepth, RagidObject& ragid);
	void checkWall(float wallHalfWidth, float wallHalfDepth, float floor, float ceiling, RagidObject & ragid);
	Vector3f calcReflex(const Vector3f & v1, const Vector3f & action, float m, float restitution);
	Vector3f calcImpulse(const Vector3f& v1, const Vector3f v2, const Vector3f &action, float m1, float m2, float restitution);
	typedef Object::Vertex Vertex;
	//接ベクトルの計算
	Vector3f calcTangent(const Vertex & v0, const Vertex & v1, const  Vertex & v2);
	Vector3d calcTangent(const std::array<double, 11> v0, const std::array<double, 11> v1, const std::array<double, 11>& v2);
	//面ごとに法線計算
	Vector3f calcNormal(const Vertex & v0, const Vertex & v1, const  Vertex & v2);
	int sign(float f);
	int sign(double d);
	int sign(int i);
}