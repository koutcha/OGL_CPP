#pragma once
#include <GL/glew.h>
//figure data
class Object2D {

public:
	//vertex Attribute
	struct Vertex
	{
		GLfloat position[2];
		GLfloat color[3];
	};
	//constructer
	// size:dimension of vertex's poisiton
	//vertexcount: vertex number
	//vertex:the array vertex's attributes put into 
	Object2D(GLint size, GLsizei vertexcount, const Vertex *vertex, GLsizei indexcount = 0, const GLuint *index = nullptr)
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
		//color
		glVertexAttribPointer(1, size, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<char*>(0) + sizeof vertex->position);
		glEnableVertexAttribArray(1);

		//index buffer object
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			indexcount * sizeof(GLuint), index, GL_STATIC_DRAW);
	}
	virtual ~Object2D()
	{
		//delete vertex array obj
		glDeleteBuffers(1, &vao);
		//delete vertex buffer obj
		glDeleteBuffers(1, &vbo);
		//delete index buffer obj
		glDeleteBuffers(1, &ibo);
	}

	void bind()const{glBindVertexArray(vao);}
private:
	//the vertex array object name
	GLuint vao;
	//the vertex buffer object name
	GLuint vbo;
	//index buffer object;
	GLuint ibo;
	//forbit to copy
	Object2D(const Object2D&);
	//forbit to copy by substitution
	Object2D &operator=(const Object2D &o) {};
};