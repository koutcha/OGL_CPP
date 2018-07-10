#pragma once
#include <memory>
#include"ColliderObject.h"
#include "Vector3.h"
#include "GL\glew.h"
class Ragid;
class Shape;
using namespace std;
class RagidObject
{
public:
	RagidObject(shared_ptr<Shape> shape);
	virtual ~RagidObject();

	virtual void draw(GLuint modelLoc) const;
	virtual void update(float dt);
	void setScale(const Vector3f& scale);
	virtual void setPosition(const Vector3f& position);
	virtual void setVelocity(const Vector3f& velocity);
	virtual void setRotation(float angle, Vector3f& rotationAxis);
	void addRotation(float angle, Vector3f & rotationAxis);
	virtual void setAcceleration(const Vector3f& acceleration);
	virtual void setWeight(float f);
	virtual void setFriction(float friction);
	virtual void addForce(const Vector3f & f);
	virtual void addGravity(const Vector3f& g);
	virtual void addImpluse(const Vector3f & ft);

	virtual const Vector3f getVelocity()const;
	virtual const Vector3f getPosition()const;
	float getWeight()const;
private:
	Vector3f shapeScale;
	shared_ptr<Shape> shape;
	unique_ptr<Ragid> body;
};
