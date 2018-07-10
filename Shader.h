#pragma once
class Shader
{
public:
	Shader();
	virtual ~Shader();
	void bind()const;
	void unBind()const;
};

