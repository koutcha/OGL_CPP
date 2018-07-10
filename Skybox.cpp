#include "Skybox.h"
#include "CubeMap.h"
#include "stb_image.h"
#include <memory>
#include <iostream>
#include "InitShader.h"
#include "Camera.h"
Skybox::Skybox(std::array<std::string, 6> faces) :
	program(loadProgram("skybox.vert", "skybox.frag")),
	cubeMap(std::make_unique<CubeMap>(faces)),
	vao(-1),
	vbo(-1),
	halfScale(1.0f)
{
	constexpr float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	
	//skybos vao
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}

Skybox::~Skybox()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vao);
	//shaderのリソース管理クラス作りましょう
	glDeleteProgram(program);
}
//シェーダーとカメラをセットしてから
void Skybox::draw(const Camera& camera)const
{
	glDepthFunc(GL_LEQUAL);
	glUseProgram(program);
	cubeMap->bind(0);
	camera.setCamera(glGetUniformLocation(program,"view"), glGetUniformLocation(program,"projection"));
	glBindVertexArray(vao);
	cubeMap->bind(0);
	glUniform1i(glGetUniformLocation(program, "skybox"), 0);
	glUniform1f(glGetUniformLocation(program, "scale"), halfScale);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

void Skybox::bindThisCubeMap(int unit) const
{
	cubeMap->bind(unit);
}

void Skybox::setHalfScale(float f)
{
	halfScale = f;
}

GLuint Skybox::getVeiwLoc() const
{
	return GLuint();
}

GLuint Skybox::getProjectionLoc() const
{
	return GLuint();
}
