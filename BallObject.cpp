#include "BallObject.h"
#include "ColliderSphere.h"
#include "MathAndPhysic.h"
#include "Matrix.h"
#include <iostream>
#include <random>
BallObject::BallObject(const shared_ptr<const Shape>& shape,float r):
	RagidObject(shape),
	collider(new ColliderSphere()),
	time(0)
{
	collider->setRadius(r);
	setScale(Vector3f(r, r, r));
}


BallObject::~BallObject()
{
}

float BallObject::getRadius() const
{
	return collider->getRadius();
}

void BallObject::setPosition(const Vector3f &v)
{
	RagidObject::setPosition(v);
	collider->setPosition(v);
}

const ColliderSphere & BallObject::getCollider() const
{
	return *collider;
}

void BallObject::update(float dt)
{
	time += dt;
	//回転の更新
	Vector3f v = RagidObject::getVelocity();
	Matrix m(Matrix::rotateY(90));
	v.y = 0.f;
	float len = v.length()*dt;
	v = m * v;
	
	RagidObject::addRotation(len/getRadius(), v);
	//玉の位置の更新
	RagidObject::update(dt);
	collider->setPosition(RagidObject::getPosition());
}

void collisionResponseForBalls(const shared_ptr<BallObject> ball0, const shared_ptr<BallObject> ball1)
{
	Vector3f Ft;
	Ft = MyMath::calcImpulse(ball0->getVelocity(), ball1->getVelocity(), ball1->getPosition() - ball0->getPosition(), ball0->getWeight(),ball1->getWeight(), 1.0);
	ball0->addImpluse(Ft);
	Ft *= -1.0;
	ball1->addImpluse(Ft);
	correctShinking(ball0, ball1);
}

void correctShinking(const shared_ptr<BallObject> ball0,const  shared_ptr<BallObject> ball1)
{
	std::random_device rnd;
	Vector3f position0(ball0->getPosition());
	Vector3f position1(ball1->getPosition());

	Vector3f line(position0);
	line -= position1;
	
	float rR = ball0->getRadius() + ball1->getRadius();
	float lineLength = line.length();
	float subLength = rR - lineLength;
	if (subLength < 0) {
		cout << rR << endl;
		std::cout << line.length()<< endl;
		std::cout << "bye" << std::endl;
		return;
	}
	if (lineLength == 0)
	{
		line = Vector3f(rnd(),0, rnd());
	}
	line.normalize();
	float bias = 0.0000000000001;
	line *= subLength + bias;
	position0 += line;
	ball0->setPosition(position0);
	position1 -= line;
	ball1->setPosition(position1);


}
