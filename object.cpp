#include "object.h"

void Object::bind() const
{

	glBindVertexArray(vao);
}

void Object::unbind() const
{
	glBindVertexArray(0);
}
