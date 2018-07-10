#pragma once
#include "Vector3.h"
#include  "Quaternion.h"
class Ragid
{
public:

	Ragid();
	Ragid(float weight,const Vector3f& iniVelocity);
	~Ragid();

	void setPosition(const Vector3f& position);
	void setVelocity(const Vector3f& velocity);


	void setRotation(const float, const Vector3f&);
	void addRotation(const float, const Vector3f&);

	void setAcceleration(const Vector3f& acceleration);
	void setGravity(const Vector3f g);
	void setWeight(float weight);
	void setFriction(float friction);
	void addForce(const Vector3f & f);
	void addGravity(const Vector3f & g);
	void addImpulse(const Vector3f& f);
	void update(float dt);

	Vector3f getVelocity()const;
	Vector3f getPosition()const;

	Quaternion getRotation() const;

	

	float getWeight()const;
private:

	Quaternion rotation;

	Vector3f position;
	Vector3f velocity;
	Vector3f accel;

	Vector3f gravity;

	float weight;
	float friction;
};

