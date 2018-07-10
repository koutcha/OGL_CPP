#version 330 core

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;



layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;




out vec3 P;
out vec3 N;
out vec2 UV;

out vec4 LSP;

void main()
{
	P = vec3(model*vec4(position,1.0));

	N =  transpose(inverse(mat3(model)))*normal;
	UV = uv;
	LSP= lightSpaceMatrix*vec4(P,1.0);
	gl_Position = projection*view*model*vec4(position, 1.0);
}