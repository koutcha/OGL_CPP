#pragma once
#include <memory>

#include "object.h"

class Shape
{
	std::shared_ptr<const Object> object;

protected:
	const GLsizei vertexcount;
public:
	Shape(GLint size, GLsizei vertexcount, const Object::Vertex *vertex,GLsizei indexcount = 0,const GLuint *index = nullptr);
	virtual ~Shape();
	void draw()const;
	virtual void execute()const;
};