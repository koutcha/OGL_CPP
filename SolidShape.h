#pragma once

#include "Shape.h"
class SolidShape:
	public Shape
{
public:
	SolidShape(GLint size,GLsizei vertexcount,const Object::Vertex *vertex);
	virtual ~SolidShape();

	virtual void execute() const override;
};

