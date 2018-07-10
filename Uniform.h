#pragma once
#include <memory>
#include "GL\glew.h"

template<typename T>
class Uniform
{
public:
	//data: data set into block
	// count : the number of uniform blocks aligned
	Uniform(const T* data = nullptr,unsigned int count = 1)
		:buffer(new UniformBuffer(data,count))
	{

	}
	virtual ~Uniform()
	{

	}
	void set(const T *data,unsigned int start = 0,unsigned int count = 1)const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, buffer->ubo);
		for (unsigned int i = 0; i < count; ++i) {
			glBufferSubData(GL_UNIFORM_BUFFER, i*blockSize, sizeof(T), data+i);
		}
		
	}
	// i:the positon of the uniform block to bind
	// bp:binding point
	void select(unsigned int i= 0,GLuint bp = 0)const
	{
		glBindBufferRange(GL_UNIFORM_BUFFER, bp, buffer->ubo, i*buffer->blockSize, sizeof(T));
	}
private:
	struct UniformBuffer
	{
		//ubo name
		GLuint ubo;
		//block size
		GLsizeiptr blockSize;
		// data:the data set into uniform object
		// count:the number of uniform blocks aligned 
		UniformBuffer(const T *data,unsigned int count)
		{
			//calc uniform block size
			GLint alignment;
			glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);
			blockSize = (((sizeof(T) - 1) / alignment) + 1)*alignment;
			glGenBuffers(1, &ubo);
			glBindBuffer(GL_UNIFORM_BUFFER, ubo);
			glBufferData(GL_UNIFORM_BUFFER, blockSize*count,NULL, GL_STATIC_DRAW);
			for (unsigned int i = 0; i < count; ++i)
			{
				glBufferSubData(GL_UNIFORM_BUFFER, i*blockSize, sizeof(T), data + i);
			}
		}
		
		~UniformBuffer()
		{
			glDeleteBuffers(1, &ubo);
		}
	};
	const std::shared_ptr<const UniformBuffer> buffer;
};