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
	//�����`����_�ւ̍ŒZ�x�N�g�������߂�
	Vector3f d = MyMath::Vector3AABBToPoint(cubeCol.getPosition(), cubeCol.getHalfSize(), sphereCol.getPosition());
	//�����߂荞��ł��鋗�������߂�
	double sinkLength = sphereCol.getRadius() - d.length();
	d.normalize();
	//d����p�x�N�g���Ƃ���Փˎ��̗͐�(�����`��m=���Av=0�Ƃ���j�����Ƃ߂�
	Vector3f ft = MyMath::calcReflex(ball0->getVelocity(), d, ball0->getWeight(), restitution);
	ball0->addImpluse(ft);
	//�߂荞�݂̏C��
	d *= sinkLength;
	d += ball0->getPosition();
	ball0->setPosition(d);
}
