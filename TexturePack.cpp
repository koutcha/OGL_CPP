#include "TexturePack.h"
#include "stb_image.h"
#include <iostream>
TexturePack::TexturePack(const std::string* filenames, unsigned int filecount):
	filecount(filecount),
	textureID(std::make_unique<GLuint[]>(filecount))
{
	glGenTextures(filecount, textureID.get());
	for (int i = 0; i < filecount; ++i) 
	{
		
		int width, height, numComponents;
		unsigned char* imageData = stbi_load(filenames[i].c_str(), &width, &height, &numComponents, 4);

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureID[i]);

		if (imageData == nullptr)
			std::cerr << "can't file open" << std::endl;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

		stbi_image_free(imageData);
	}
}

TexturePack::~TexturePack()
{
	glDeleteTextures(2, textureID.get());
}

void TexturePack::bind()
{
	

}