#include "Textureh.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>
Texture::Texture(const std::string & filename)
{
 int width, height, numComponents;
	unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &numComponents, 4);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	if (imageData == nullptr)
		std::cerr << "can't file open:" << filename <<std::endl;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,imageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(imageData);
}
Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

GLuint Texture::getID() const
{
	return textureID;
}

void Texture::bind(unsigned int unit)const
{
		//tex0 is binded by shadow map so the number of unit should be set on over 0
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, textureID);

}




