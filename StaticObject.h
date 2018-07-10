#pragma once
#include "Vector3.h"
#include "GL\glew.h"

#include <memory>
class Shape;
class StaticObject
{
public:
	StaticObject(std::shared_ptr<Shape> shape);
	virtual ~StaticObject();
	virtual void draw(GLuint modelLoc)const;
	virtual void setPosition(const Vector3f& position);
	virtual void setRotation(const Vector3f& rotation);
	virtual void setScale(const Vector3f& scale);

	virtual const Vector3f getPositon()const;
	virtual const Vector3f getRotation()const;
	virtual const Vector3f getScale()const;
private:
	Vector3f shapeScale;
	Vector3f position;
	Vector3f rotation;
	std::shared_ptr<Shape> shape;
};

