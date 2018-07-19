#include "MainLight.h"
#include "GL\glew.h"
#include "Shader.h"

MainLight::MainLight()
{
}

MainLight::MainLight(const Vector3f & position, const Vector3f & diffuse, const Vector3f & ambient, const Vector3f & specular):
	position(position),
	diffuse(diffuse),
	ambient(ambient),
	specular(specular),
	uniformNames{"Lpos","Ldiff","Lamb","Lspec"}
{
}


MainLight::~MainLight()
{
}



void MainLight::setPosition(const Vector3f &p)
{
	position = p;
}

void MainLight::setDiffuse(const Vector3f &d)
{
	diffuse = d;
}

void MainLight::setAmbient(const Vector3f &a)
{
	ambient = a;
}

void MainLight::setSpecular(const Vector3f &s)
{
	specular = s;
}

const Vector3f MainLight::getPostiion() const
{
	return position;
}

const Vector3f MainLight::getDiffuse() const
{
	return diffuse;
}

const Vector3f MainLight::getAmbient() const
{
	return ambient;
}

const Vector3f MainLight::getSpecular() const
{
	return specular;
}

void MainLight::setOnShader(const Shader& shader)const
{
	glUniform3fv(shader.getUniformLoc(uniformNames[0]), 1, position.getArrayData().data());
	glUniform3fv(shader.getUniformLoc(uniformNames[1]), 1, diffuse.getArrayData().data());
	glUniform3fv(shader.getUniformLoc(uniformNames[2]), 1, ambient.getArrayData().data());
	glUniform3fv(shader.getUniformLoc(uniformNames[3]), 1, specular.getArrayData().data());
}
void MainLight::setPositionNameOnShader(const std::string &s)
{
	uniformNames[0] = s;
}
void MainLight::setDiffuseNameOnShader(const std::string &s)
{
	uniformNames[1] = s;
}

void MainLight::setAmbientNameOnShader(const std::string &s)
{
	uniformNames[2] = s;
}

void MainLight::setSpecularNameOnShader(const std::string &s)
{
	uniformNames[3] = s;
}
