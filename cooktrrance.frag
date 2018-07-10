#version 330 core

uniform vec3 Lpos;
uniform vec3 Lspec;
uniform vec3 Lamb;
uniform vec3 Ldiff;

uniform vec3 viewPos;

uniform sampler2D shadowMap;
uniform sampler2D diffuseTex;
uniform sampler2D skin;
uniform bool skinTexOn;
uniform sampler2D normalMap;
uniform bool normalMapOn;


layout (std140) uniform Material
{
	vec3 Kamb;
	vec3 Kdiff;
	vec3 Kspec;
	float m;
	float refrac;
};

in vec3 P;
in vec3 N;
in vec2 UV;
in vec4 LSP;

in  vec3 TLpos;
in  vec3 TVpos;
in  vec3 Tpos;


out vec4 fragment;

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

void main()
{
	vec3 normal =normalMapOn? texture(normalMap,UV).rgb : vec3(0,0,1);
	//normal mapÇ©ÇÁñ@ê¸éÊìæ îÕàÕ[0,1]
	//normal = normalize(normal);

	//[-1,1]Ç…ïœä∑
	if(normalMapOn)
	normal = normalize(normal*2.0 -1.0) ;

	vec3 Iamb = Kamb*Lamb;

	vec3  L = normalize(TLpos - Tpos);

	float diff = max(dot(L,normal),0.0);

	vec4 tempDiff = skinTexOn ?texture(skin,UV):vec4(Kdiff,1.0);
	vec3 Idiff = vec3(tempDiff)*Ldiff ;

	Idiff *= diff;

	vec3 V = normalize(TVpos - Tpos);
	vec3 H = normalize(L+V);


	//micro faset
	float dotNH = dot(normal,H);

	//let N and H be normalized
	float dotNH2 = dotNH*dotNH;
	float m2 = m*m;
	float e = exp((dotNH2 - 1.0)/(m2*dotNH2));
	float D =  e/(4*m2*dotNH2*dotNH2);

	//geometry decay
	float dotVH = dot(V,H);
	float dotNV = dot(normal,V);
	float Ga = 2.0*dotNH*dotNV/dotVH;
	float Gb = 2.0*dotNH*dot(normal,L)/dotVH;
	
	float G = min(min(Ga,Gb),1.0);
	//fresnel
	float c = dot(L,H);
	float w = pow(1.0 - max(0.0,c),5.0);

	float F  = refrac+(1.0-refrac)*w;


	float spec = D*G*F/dotNV;
	vec3 Ispec = Kspec*Lspec;
	Ispec *= spec;
	float shadow = ShadowCalculation(LSP);
	vec3 temp = (Idiff+Ispec)*(1.0 - shadow*0.7)+ Iamb;

	fragment = vec4(temp,tempDiff.w);
}