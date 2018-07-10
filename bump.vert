#version 330 core

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

uniform vec3 Lpos;
uniform vec3 viewPos;


const int PLMax = 4;
uniform vec3 PLpos[PLMax];
uniform int pointLightsNum;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tangent;

out vec3 P;
out vec2 UV;
out vec3 N;

out  vec3 TLpos;
out  vec3 TVpos;
out vec3 Tpos;

out vec3 TPLpos[PLMax];

out vec4 LSP;

void main()
{
	P = vec3(model*vec4(position,1.0));
	UV = uv;
	LSP= lightSpaceMatrix*vec4(P,1.0);

    vec3 n = normalize(normal);
	vec3 t = normalize(tangent);
	mat3 nMat = transpose(inverse(mat3(model)));
	N =  normalize(nMat*n);
	vec3 T = normalize(nMat*t);
	T = normalize(T - dot(T,N)*N);

	vec3 B = cross(N,T);

	mat3 TBN = transpose(mat3(T,B,N));

	TLpos = TBN*Lpos;
	TVpos = TBN*viewPos;
	Tpos = TBN*P;

	gl_Position = projection*view*model*vec4(position, 1.0);
}
