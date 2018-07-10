#pragma once
#include "GL\glew.h"
#include "Matrix.h"
class ShadowFBO
{
	static const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	GLuint depthMapFBO;
	GLuint depthMap;
	GLuint shaderProgram;

	Matrix pMat;
	Matrix vMat;

public:
	ShadowFBO();
	~ShadowFBO();


	const Matrix getLightSpaceMat()const;
	void setLightSpaceMatrixToShader(GLuint LSMLocation)const;
	void setProjection(const Matrix & m);
	void setView(const Matrix& m);
	void setShadowMap(unsigned int texUnitNum)const;
	void unbind()const;
	void execute()const;

	GLuint getModelLocation()const;
};

