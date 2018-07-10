#version 150 core
uniform mat4 modelview;
uniform mat4 projection;
uniform mat3 normalMatrix;

const int Lcount = 2;
uniform vec4 Lpos[Lcount];
uniform vec3 Lamb[Lcount];
uniform vec3 Ldiff[Lcount];
layout (std140) uniform Material
{
	vec3 Kamb;
	vec3 Kdiff;
	vec3 Kspec;
	float Kshi;
};
in vec4 position;
in vec3 normal;
in vec2 uv;

out vec3 Idiff;
out vec4 P;
out vec3 N;
out vec2 UV;

out vec4 shadow;

void main()
{
	 P = modelview * position;
	 N = normalize(normalMatrix * normal);
	 UV = uv;
	gl_Position = projection * P;
}