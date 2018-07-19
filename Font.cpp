#include "InitShader.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "Font.h"
#include <iostream>
#include <iomanip>
#include "Matrix.h"
char* file_read(const char* filename)
{
	FILE* in;
	fopen_s(&in,filename, "rb");
	if (in == NULL) return NULL;

	int res_size = BUFSIZ;
	char* res = (char*)malloc(res_size);
	int nb_read_total = 0;

	while (!feof(in) && !ferror(in)) {
		if (nb_read_total + BUFSIZ > res_size) {
			if (res_size > 10 * 1024 * 1024) break;
			res_size = res_size * 2;
			res = (char*)realloc(res, res_size);
		}
		char* p_res = res + nb_read_total;
		nb_read_total += fread(p_res, 1, BUFSIZ, in);
	}

	fclose(in);
	res = (char*)realloc(res, nb_read_total + 1);
	res[nb_read_total] = '\0';
	return res;
}



Font::Font(const char* filepath):
	filepath(filepath)
{

}

Font::~Font()
{
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(program);
}

void Font::init()
{
	/* Initialize the FreeType2 library */
	if (FT_Init_FreeType(&ft)) {
		std::cerr << "Could not init freetype library" << std::endl;
		return;
	}

	/* Load a font */
	char* font = file_read(filepath.c_str());
	if (FT_New_Face(ft, filepath.c_str(), 0, &face)) {
		std::cerr << "Could not open Font" << std::endl;
	}
	program = loadProgram("text.vert", "text.frag");

	if (program == 0) 
	{
		std::cerr << "font Init is failed" << std::endl;
		return;
	}
	FT_Set_Pixel_Sizes(face, 0, 48);


	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			{face->glyph->bitmap.width, face->glyph->bitmap.rows},
			{face->glyph->bitmap_left, face->glyph->bitmap_top },
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
void Font::renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale,GLfloat color[3],GLfloat width,GLfloat height)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(program);
	Matrix projection = Matrix::orthogonal(0.0f,width, 0.0f, height,-10,10);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE,projection.data());

	glUniform3f(glGetUniformLocation(program, "textColor"), color[0], color[1], color[2]);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);
	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing[0] * scale;
		GLfloat ypos = y - (ch.Size[1] - ch.Bearing[1]) * scale;

		GLfloat w = ch.Size[0] * scale;
		GLfloat h = ch.Size[1] * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos,     ypos,       0.0, 1.0 },
		{ xpos + w, ypos,       1.0, 1.0 },

		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos + w, ypos,       1.0, 1.0 },
		{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void Font::bindFontTexture(unsigned int unitNumber)
{
	glActiveTexture(GL_TEXTURE0 + unitNumber);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

