#pragma once
#include "Unocopyable.h"
#include <string>
#include <GL/glew.h>

class Texture:private UnCopyable
{
public:
	Texture(const std::string& filename);
	virtual ~Texture();
	GLuint getID()const;
	void bind(unsigned int unit)const;
private:
	GLuint textureID;
};