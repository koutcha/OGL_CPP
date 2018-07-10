#pragma once
#include<memory>
#include <string>
#include <map>
#include <GL\glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H
class SolidShape;
class Font {

	struct Character {
		GLuint TextureID;   // ID handle of the glyph texture
		GLfloat Size[2];    // Size of glyph
		GLfloat Bearing[2];  // Offset from baseline to left/top of glyph
		GLuint Advance;    // Horizontal offset to advance to next glyph
	};


	//ft lib handle
	FT_Library ft;
	//face object handle
	FT_Face face;
	std::map<GLchar, Character> Characters;
	std::string filepath;
	GLuint program;
	GLuint texLoc;
	GLuint textureID;
	GLuint colorLoc;
	GLuint coordLoc;
	GLuint vbo,vao;
	//std::unique_ptr<SolidShape> shape;
public:
	Font(const char* filepath);
	~Font();

	void init();
	void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, GLfloat color[3], GLfloat width, GLfloat height);
	void bindFontTexture(unsigned int unitNumber);
};