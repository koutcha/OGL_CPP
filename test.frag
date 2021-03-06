#version 330 core


uniform vec3 Lpos;
uniform vec3 Lspec;
uniform vec3 Lamb;
uniform vec3 Ldiff;

uniform vec3 viewPos;

uniform sampler2D shadowMap;
uniform sampler2D skin;
uniform bool skinTexOn;

layout (std140) uniform Material
{
	vec3 Kamb;
	vec3 Kdiff;
	vec3 Kspec;
	float Kshi;
};

in vec3 P;
in vec3 N;
in vec2 UV;
in vec4 LSP;


float ShadowCalculation(vec4 LSP)
{
    // perform perspective divide
    vec3 projCoords = LSP.xyz / LSP.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	vec3 normal = normalize(N);
    vec3 lightDir = normalize(Lpos - P);
	float bias = max(0.02 * (1.0 - dot(normal, lightDir)), 0.005);

	float shadow = 0.0;

    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

	if(projCoords.z > 1.0)
    shadow = 0.0;

    return shadow;
}  

out vec4 fragment;


void main()
{
	vec3 normal = normalize(N);

	vec3 Iamb = Kamb*Lamb;

	//vec3  L = normalize(Lpos-P);
	vec3  L = normalize(Lpos);

	float diff = max(dot(L,normal),0.0);
	vec3 Idiff = skinTexOn ?vec3(texture2D(skin,UV)) :  Kdiff*Ldiff ;
	Idiff *= diff;

	vec3 V = normalize(viewPos - P);
	vec3 H = normalize(L+V);
	float spec = 0.0;
	spec = pow(max(dot(normal,H),0.0),Kshi);
	vec3 Ispec = Kspec*Lspec;
	Ispec *= spec;
	float shadow = ShadowCalculation(LSP);
	vec3 temp = (Idiff+Ispec)*(1.0 - shadow)+ Iamb;

	fragment = vec4(temp,1.0);
}