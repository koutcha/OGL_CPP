#include "Ragid.h"
#include "Vector3.h"


Ragid::Ragid():
	position(0,0,0),
	velocity(0,0,0),
	accel(0,0,0),
	gravity(0,0,0),
	weight(1.0)
{
	rotation.setRotation(Vector3f(0, 1, 0), 0);
}

Ragid::Ragid(float weight, const Vector3f & iniVelocity):
	position(0, 0, 0),
	velocity(iniVelocity),
	accel(0, 0, 0),
	weight(weight)
{
	rotation.setRotation(Vector3f(0, 1, 0), 0);
}




Ragid::~Ragid()
{
}

void Ragid::setPosition(const Vector3f & p)
{
	position = p;
}

void Ragid::setVelocity(const Vector3f & v)
{
	velocity = v;
}



void Ragid::setRotation(const float angle, const Vector3f& axis)
{
	rotation.setRotation(axis, angle);
}
void Ragid::addRotation(const float angle, const Vector3f& axis)
{
	//rotation.setRotation(axis, angle);

	Quaternion q;
	q.setRotation(axis,angle);
	//q.setRotation(axis, angle);
	rotation = q*rotation;
}
void Ragid::setAcceleration(const Vector3f & acceleration)
{
	accel = acceleration;
}
void Ragid::setGravity(const Vector3f g)
{
}
void Ragid::setWeight(float m)
{
	weight = m;
}
void Ragid::setFriction(float mu)
{
	friction = mu;
}
void Ragid::addForce(const Vector3f &f)
{
	Vector3f tempA(f);
	tempA *= 1.0 / weight;
	accel += tempA;
}
void Ragid::addGravity(const Vector3f &g)
{
	accel += g;
}
void Ragid::addImpulse(const Vector3f & f)
{
	Vector3f temp = f;
	temp /=  weight;
	velocity += temp;
}

void Ragid::update(float dt)
{


	//euler method
	Vector3f da(accel);
	//da += gravity;
	da *= dt;
	if (friction > 0.f && velocity.length() > 0.0) {

		Vector3f temp = velocity;
		float nf = friction*weight * 9.8;
		temp.normalize();
		temp *= -nf;
		temp *= dt;
		if (temp.length() > velocity.length())
		{
			velocity = Vector3f(0, 0, 0);
		}
		else {
			velocity += temp;
		}

	}
	velocity += da;
	Vector3f dp(velocity);
	dp *= dt;
	position += dp;
	accel = Vector3f(0, 0, 0);
}

Vector3f Ragid::getVelocity() const
{
	return velocity;
}

Vector3f Ragid::getPosition() const
{
	return position;
}

Quaternion Ragid::getRotation() const
{
	return rotation;
}



float Ragid::getWeight() const
{
	return weight;
}
