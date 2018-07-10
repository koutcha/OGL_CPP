#include "Shape.h"

Shape::Shape(GLint size, GLsizei vertexcount, const Object::Vertex * vertex,GLsizei indexcount,const GLuint *index):
	object(new Object(size,vertexcount,vertex,indexcount,index)),
	vertexcount(vertexcount)
{

}

Shape::~Shape()
{
}

void Shape::draw()const
{
	object->bind();
	execute();
	object->unbind();
}

void Shape::execute()const
{
	glDrawArrays(GL_LINE_LOOP, 0, vertexcount);
}
