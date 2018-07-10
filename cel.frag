#version 330 core

//precision mediump float;

uniform vec3 Lpos;
uniform vec3 Lspec;
uniform vec3 Lamb;
uniform vec3 Ldiff;

uniform vec3 viewPos;

uniform sampler2D shadowMap;
uniform sampler2D celSample;
uniform sampler2D skin;
uniform bool skinTexOn;
uniform sampler2D normalMap;
uniform bool normalMapOn;

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

in  vec3 TLpos;
in  vec3 TVpos;
in vec3 Tpos;


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
	//sampling around texcel on shadow map
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
	vec3 normal =normalMapOn? texture(normalMap,UV).rgb : vec3(0,0,1);
	//normal map‚©‚ç–@üŽæ“¾ ”ÍˆÍ[0,1]
	//normal = normalize(normal);
	
	//normal = vec3(0,0,1);
	//normal = N;
	//[-1,1]‚É•ÏŠ·
	if(normalMapOn)
	normal = normalize(normal*2.0 -1.0);

	vec3 Iamb = Kamb*Lamb;

	//vec3  L = normalize(Lpos-P);
	vec3  L = normalize(TLpos - Tpos);
	float diff = clamp(dot(L,normal),0.0,1.0);

	//float alpha = texture2D(skin,UV).a;
	//vec3 Tdiff =  vec4(1, 1, 1, texture2D(tex, texpos).a);
	//sampleing texture
	vec3 smpColor = texture2D(celSample,vec2(diff,0.0)).xyz;
	vec4 tempDiff = skinTexOn ?texture(skin,UV):vec4(Kdiff,1.0);
	vec3 Idiff = vec3(tempDiff)*smpColor;

	vec3 V = normalize(TVpos - Tpos);
	vec3 H = normalize(L+V);
	float spec = 0.0;
	
	spec = pow(max(dot(normal,H),0.0),Kshi);
	//vec3 Ispec = Kspec*Lspec;
	vec3 Ispec  = spec > 0.9 ? vec3(1.0) : vec3(0.0);
	float shadow = ShadowCalculation(LSP);

	//vec3 temp = (Idiff+Ispec)*(1.0 - shadow)+ Iamb;
	vec3 temp = (Idiff+Ispec)*(1.0-shadow*0.7);
	fragment = vec4(temp,tempDiff.w);
}