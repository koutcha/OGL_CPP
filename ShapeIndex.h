#pragma once

#include "Shape.h"
class ShapeIndex:
	public Shape
{
public:
	ShapeIndex(GLint size,GLsizei veetexcount,const Object::Vertex *vertex,GLsizei indexcount,const GLuint *index);
	virtual ~ShapeIndex();

	virtual void execute() const;
protected:
	const GLsizei indexcount;

};

