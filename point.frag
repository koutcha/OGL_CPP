//shadertype=<glsl>
#version 150 core
const int Lcount = 2;
uniform vec4 Lpos[Lcount];
uniform vec3 Lspec[Lcount];
uniform vec3 Lamb[Lcount];
uniform vec3 Ldiff[Lcount];

uniform sampler2D tex0;
uniform sampler2D tex1;

layout (std140) uniform Material
{
	vec3 Kamb;
	vec3 Kdiff;
	vec3 Kspec;
	float Kshi;
};
in vec3 Idiff;
in vec4 P;
in vec3 N;
in vec2 UV;
out vec3 fragment;

void main()
{
	
	vec3 V = -normalize(P.xyz);
	vec3 Ispec = vec3(0.0);
	vec3 Idiff = vec3(0.0);
	vec4 Tdiff4 = (P.x >0) ? vec4(texture2D(tex0,UV)) : vec4(texture2D(tex1,UV));
	vec3 Tdiff = vec3(Tdiff4);
	for (int i = 0; i < Lcount; ++i)
	{
		vec3 L = normalize((Lpos[i] * P.w - P * Lpos[i].w).xyz);
		vec3 Iamb = Kamb * Lamb[i];
		Idiff += max(dot(N, L), 0.0) *Kdiff * Ldiff[i] + Iamb;
	}
	for(int i=0; i<Lcount; ++i)
	{
		vec3 L = normalize((Lpos[i]*P.w - P*Lpos[i].w).xyz);
		vec3 H = normalize(L+V);
		Ispec += pow(max(dot(normalize(N), H), 0.0), Kshi) * Kspec * Lspec[i];
	}
	vec3 temp = Idiff+Ispec;


	fragment =temp;
}