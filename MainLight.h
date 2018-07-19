#pragma once
#include "Vector3.h"
class Shader;
class MainLight
{
public:
	MainLight();
	MainLight(const Vector3f& position, const Vector3f& diffuse, const Vector3f& ambient, const Vector3f& specular);
	~MainLight();


	void setPositionNameOnShader(const std::string&);
	void setDiffuseNameOnShader(const std::string&);
	void setAmbientNameOnShader(const std::string&);
	void setSpecularNameOnShader(const std::string&);

	void setPosition(const Vector3f&);
	void setDiffuse(const Vector3f&);
	void setAmbient(const Vector3f&);
	void setSpecular(const Vector3f&);


	const Vector3f getPostiion()const;
	const Vector3f getDiffuse()const;
	const Vector3f getAmbient()const;
	const Vector3f getSpecular()const;


	void setOnShader(const Shader& shader)const;
private:
	std::array<std::string, 4> uniformNames;
	Vector3f position;
	Vector3f diffuse;
	Vector3f ambient;
	Vector3f specular;
};

