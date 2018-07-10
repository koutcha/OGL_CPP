#pragma once
#include "GL\glew.h"
#include "Unocopyable.h"
#include "CubeMap.h"

#include <array>
#include <memory>
#include <string>
class Camera;
class Skybox
{
public:
	Skybox(std::array<std::string, 6> faces);
	~Skybox();
	void draw(const Camera & camera) const;
	void bindThisCubeMap(int unit)const;
	void setHalfScale(float f);

	GLuint getVeiwLoc()const;
	GLuint getProjectionLoc()const;
private:
	GLuint program;
	std::unique_ptr<CubeMap> cubeMap;
	GLuint vao;
	GLuint vbo;

	float halfScale;
};

