#pragma once
#include <GL/glew.h>
#include <array>
//figure data
class Object {

public:
	//vertex Attribute
	struct Vertex
	{

		std::array<GLfloat, 3> position;
		std::array<GLfloat, 3> normal;
		std::array<GLfloat, 2> uv;
		std::array<GLfloat, 3> tangent;

	};
	//constructer
	// size:dimension of vertex's poisiton
	//vertexcount: vertex number
	//vertex:the array vertex's attributes put into 
	Object(GLint size, GLsizei vertexcount, const Vertex *vertex,GLsizei indexcount = 0,const GLuint *index= nullptr):
		size(size)
	{
		//vertex array object
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);


		//vertex buffer object
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexcount * sizeof(Vertex), vertex, GL_STATIC_DRAW);
		//enable  to refference  conbined vertex buffer object from "in" literal 
		glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(0);
		//normal
		glVertexAttribPointer(1, size, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<char*>(0) + sizeof Vertex::position);
		glEnableVertexAttribArray(1);
		//uv
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<char*>(0) + sizeof Vertex::position + sizeof Vertex::normal);
		glEnableVertexAttribArray(2);
		//tangent
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<char*>(0) + sizeof Vertex::position + sizeof Vertex::normal + sizeof Vertex::uv);
		glEnableVertexAttribArray(3);
		//index buffer object
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			indexcount*sizeof(GLuint),index,GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
	virtual ~Object()
	{
		//delete vertex array obj
		glDeleteBuffers(1, &vao);
		//delete vertex buffer obj
		glDeleteBuffers(1, &vbo);
		//delete index buffer obj
		glDeleteBuffers(1, &ibo);
	}

	void bind()const;
	void unbind()const;
private:
	GLint size;
	//the vertex array object name
	GLuint vao;
	//the vertex buffer object name
	GLuint vbo;
	//index buffer object;
	GLuint ibo;
	//forbit to copy
	Object(const Object&);
	//forbit to copy by substitution
	Object &operator=(const Object &o) {};
};