#pragma once
#include "Unocopyable.h"
#include"GL\glew.h"
#include<string>
#include<memory>
class TexturePack:private UnCopyable
{
public:
	TexturePack(const std::string* filenames,unsigned int filecount);
	~TexturePack();
	void bind();
private:
	unsigned int filecount;
	std::unique_ptr<GLuint[]> textureID;
};

