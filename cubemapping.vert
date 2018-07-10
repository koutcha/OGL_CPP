#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 P;
out vec3 N;



void main(void){
	P = (model*vec4(position,1.0)).xyz;
	
	mat3 nMat = transpose(inverse(mat3(model)));
	N = nMat*normalize(normal);
	gl_Position = projection*view*model* vec4(position, 1.0);
}