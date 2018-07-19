#include "Shader.h"
#include "InitShader.h"
#include <iostream>


Shader::Shader(const std::string & vert, const std::string & frag):
	programID(0),
	isAvailable(false)
{
	set(vert, frag);

}

Shader::Shader(const std::string & vert, const std::string & geom, const std::string & frag)
{
	set(vert, geom, frag);
}

Shader::~Shader()
{
	isAvailable = false;
	glDeleteProgram(programID);
}

void Shader::set(const std::string & vert, const std::string & frag)
{
	if (isAvailable)
	{
		glDeleteProgram(programID);
	}
	if (programID = loadProgram(vert.c_str(), frag.c_str()))
	{
		std::cout << vert << "suceed" << std::endl;
		isAvailable = true;
	}
	else
	{
		isAvailable = false;
	}

}

void Shader::set(const std::string & vert, const std::string & geom, const std::string & frag)
{
	if (isAvailable)
	{
		glDeleteProgram(programID);
	}
	if (programID = loadProgram(vert.c_str(),geom.c_str(), frag.c_str()))
	{
		isAvailable = true;
	}
	else
	{
		isAvailable = false;
	}
}

bool Shader::checkIsAvailable() const
{
	if (!isAvailable)
	{
		std::cerr << "can't use program" << std::endl;
		return false;
	}
	else
	{
		true;
	}
}

void Shader::use() const
{
	//if (isAvailable)
		//return;

	glUseProgram(programID);
}

void Shader::unUse() const
{
	glUseProgram(0);
}


GLuint Shader::getUniformLoc(const std::string & name)const
{
	//if (!isAvailable)
		//return 0;

	return glGetUniformLocation(programID, name.c_str());
}

void Shader::blockBinding(const std::string & name, unsigned int unit) const
{
	//if (!isAvailable)
		//return;

	glUniformBlockBinding(programID, glGetUniformBlockIndex(programID, name.c_str()), unit);
}


