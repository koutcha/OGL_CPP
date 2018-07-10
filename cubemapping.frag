#version 330 core

uniform vec3 viewPos;
uniform samplerCube cubeTexture;

uniform bool isReflection;

in vec3 P;
in vec3 N;

out vec4 fragment;

void main(void)
{

	vec3 ref  = isReflection? reflect(P - viewPos,N) : normalize(N);
	//vec3 ref = N;
	vec3 color = texture(cubeTexture,ref).xyz;

	fragment = vec4(color,1.0);
}