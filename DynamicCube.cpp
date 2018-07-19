#include "DynamicCube.h"
#include "ColliderCube.h"
#include "BallObject.h"
#include "ColliderSphere.h"
#include "MathAndPhysic.h"
DynamicCube::DynamicCube(const std::shared_ptr<const Shape>& shape,const  Vector3f& halfSize):
	RagidObject(shape),
	collider(make_unique<ColliderCube>())
{
	collider->setHalfSize(halfSize);
	RagidObject::setScale(halfSize);
}

DynamicCube::~DynamicCube()
{
}

const ColliderCube & DynamicCube::getCollider() const
{
	return *collider;
}

Vector3f DynamicCube::getHalfSize() const
{
	return collider->getHalfSize();
}

void DynamicCube::setPosition(const Vector3f & p)
{
	collider->setPosition(p);
	RagidObject::setPosition(p);
}

void DynamicCube::update(float dt)
{
	RagidObject::update(dt);
	collider->setPosition(RagidObject::getPosition());
}

void collisionResponseForBallAndCube(const std::shared_ptr<BallObject>& ball0, const std::shared_ptr<DynamicCube>& cube, float restitution)
{
	const ColliderSphere& sphereCol = ball0->getCollider();
	const ColliderCube& cubeCol = cube->getCollider();
	//正方形から点への最短ベクトルを求める
	Vector3f d = MyMath::Vector3AABBToPoint(cubeCol.getPosition(), cubeCol.getHalfSize(), sphereCol.getPosition());
	//球がめり込んでいる距離を求める
	double sinkLength = sphereCol.getRadius() - d.length();
	d.normalize();
	//dを作用ベクトルとする衝突時の力積(正方形はm=∞、v=0とする）をもとめる
	Vector3f ft = MyMath::calcReflex(ball0->getVelocity(), d, ball0->getWeight(), restitution);
	ball0->addImpluse(ft);
	//めり込みの修正
	d *= sinkLength;
	d += ball0->getPosition();
	ball0->setPosition(d);
}
