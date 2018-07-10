#pragma once
#include "GL\glew.h"
#include <array>
class CubeMap
{
public:
	CubeMap(std::array<std::string, 6> faces);
	~CubeMap();
	void bind(unsigned int unit) const;

private:
	GLuint textureID;
};

