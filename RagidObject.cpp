#include "RagidObject.h"
#include "Ragid.h"
#include "SolidShapeIndex.h"
#include "Matrix.h"



RagidObject::RagidObject(shared_ptr<Shape> shape):
	shapeScale(1,1,1),
	shape(shape),
	body(std::make_unique<Ragid>())
{
}


RagidObject::~RagidObject()
{
}

void RagidObject::draw(GLuint modelLoc) const
{
	Vector3f v = body->getPosition();
	Quaternion q = body->getRotation();
	const Matrix model(Matrix::translate(v.x,v.y, v.z)*Matrix::rotate(q)*Matrix::scale(shapeScale.x,shapeScale.y,shapeScale.z));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());
	shape->draw();
}

void RagidObject::update(float dt)
{
	body->update(dt);
}

void RagidObject::setScale(const Vector3f & scale)
{
	shapeScale = scale;
}

void RagidObject::setPosition(const Vector3f & position)
{
	body->setPosition(position);
}

void RagidObject::setVelocity(const Vector3f & velocity)
{
	body->setVelocity(velocity);
}

void RagidObject::setRotation(float angle, Vector3f & rotationAxis)
{
	body->setRotation(angle, rotationAxis);
}
void RagidObject::addRotation(float angle, Vector3f & rotationAxis)
{
	body->addRotation(angle, rotationAxis);
}
void RagidObject::setAcceleration(const Vector3f & acceleration)
{
	body->setAcceleration(acceleration);
}
void RagidObject::setWeight(float f)
{
	body->setWeight(f);
}
void RagidObject::setFriction(float friction)
{
	body->setFriction(friction);
}
void RagidObject::addForce(const Vector3f & f)
{
	body->addForce(f);
}
void RagidObject::addGravity(const Vector3f & g)
{
	body->addGravity(g);
}
void RagidObject::addImpluse(const Vector3f& ft)
{
	body->addImpulse(ft);
}
const Vector3f RagidObject::getVelocity() const
{
	return body->getVelocity();
}

const Vector3f RagidObject::getPosition() const
{
	return body->getPosition();
}

float RagidObject::getWeight() const
{
	return body->getWeight();
}
