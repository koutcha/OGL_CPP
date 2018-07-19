#include "StaticObject.h"
#include "GL\glew.h"
#include "Matrix.h"
#include "Shape.h"
StaticObject::StaticObject(std::shared_ptr<Shape> shape):
	shape(shape),
	position(0,0,0),
	rotation(0,0,0),
	shapeScale(1,1,1)
{
}


StaticObject::~StaticObject()
{
}

void StaticObject::draw(GLuint modelLoc) const
{
	Matrix model = Matrix::translate(position.x, position.y, position.z)*
		Matrix::rotateY(rotation.y)*Matrix::rotateX(rotation.x)*Matrix::rotateZ(rotation.z)*
		Matrix::scale(shapeScale.x, shapeScale.y, shapeScale.z);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE,model.data());
	shape->draw();
}

void StaticObject::setPosition(const Vector3f & position)
{
	this->position = position;
}

void StaticObject::setRotation(const Vector3f & rotation)
{
	this->rotation = rotation;
}

void StaticObject::setScale(const Vector3f & scale)
{
	shapeScale = scale;
}

const Vector3f StaticObject::getPositon() const
{
	return position;
}

const Vector3f StaticObject::getRotation() const
{
	return rotation;
}

const Vector3f StaticObject::getScale() const
{
	return shapeScale;
}

