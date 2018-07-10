#pragma once
#include "GL\glew.h"
//frame buffer object struct
struct RxFBO
{
	GLuint id;
	int w, h, c;
	int size; //bufferSize
	GLuint texID;
	GLuint rboid;
};