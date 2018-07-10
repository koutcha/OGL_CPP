#include "ShadowFBO.h"
#include "InitShader.h"

#include<GL\glew.h>
#include <GLFW/glfw3.h>


ShadowFBO::ShadowFBO()
{
	//create fbo
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//create Texture for fbo;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//create shader for depthmap
	shaderProgram = loadProgram("shadow.vert", "shadow.frag");

}


ShadowFBO::~ShadowFBO()
{
	glDeleteTextures(1, &depthMap);
	glDeleteFramebuffers(1, &depthMapFBO);
}

const Matrix  ShadowFBO::getLightSpaceMat() const
{
	return std::move( pMat * vMat);
}

void ShadowFBO::setLightSpaceMatrixToShader(GLuint LSMLocation) const
{
	glUniformMatrix4fv(LSMLocation, 1, GL_FALSE,getLightSpaceMat().data());
}

void ShadowFBO::setProjection(const Matrix & m)
{
	pMat = m;
}

void ShadowFBO::setView(const Matrix & m)
{
	vMat = m;
}


void ShadowFBO::setShadowMap(unsigned int texUnitNum) const
{
	glActiveTexture(GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, depthMap);
}

void ShadowFBO::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowFBO::execute() const
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glViewport(0, 0, 1024, 1024);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgram);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	float near_plane = -10.f, far_plane = 40.5f;
	const Matrix lightSpaceMat(pMat*vMat);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"lightSpaceMatrix"), 1, GL_FALSE, lightSpaceMat.data());
}

GLuint ShadowFBO::getModelLocation() const
{
	return glGetUniformLocation(shaderProgram, "model");
}
