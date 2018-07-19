#pragma once
#include "GL\glew.h"
#include <string>
class Shader
{
public:
	Shader(const std::string& vert, const std::string& frag);
	Shader(const std::string& vert, const std::string& geom, const std::string& frag);
	virtual ~Shader();
	void set(const std::string& vert, const std::string& frag);
	void set(const std::string& vert, const std::string& geom, const std::string& frag);
	//保持しているシェーダープログラムが有効かどうか調べる
	bool checkIsAvailable()const;
	void use()const;
	void unUse()const;
	GLuint getUniformLoc(const std::string& name)const;
	//uniromBlockをバインド
	void blockBinding(const std::string& name,unsigned int unit)const;
private:
	GLuint programID;
	bool isAvailable;
};

