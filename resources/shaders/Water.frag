#version 330 core

in vec2 textureCoord;
in vec3 fragPos;
in vec3 normal;
out vec4 frag_color;

#define MAX_TEXTURES 11
#define MAX_POINT_LIGHTS 20
#define MAX_SPOT_LIGHTS 20
#define MAX_DIRECTION_LIGHTS 20

//lighting values
struct Material {
    sampler2D diffuseTexture;
    sampler2D specularMap;
    sampler2D emissionMap;
    float alpha;
};

float alpha = 64;

struct PointLight {
    vec3 position;
    vec3 diffuse;
    vec3 specular;

    //attenuation values
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float maxAngle;
    float featherAngle;

    vec3 diffuse;
    vec3 specular;

    //attenuation values
    float constant;
    float linear;
    float quadratic;
};

struct DirectionLight {
    vec3 direction;

    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D textures[MAX_TEXTURES];
//uniform sampler2D _MainTex;
//uniform sampler2D _FlowMap;
//uniform sampler2D _DerivHeightMapMap;


//light uniforms
uniform vec3 ambient_Light;
uniform SpotLight	   spotLights[MAX_SPOT_LIGHTS];
uniform DirectionLight directionLights[MAX_DIRECTION_LIGHTS];
uniform PointLight	   pointLights[MAX_POINT_LIGHTS];

uniform int numSpotLights;
uniform int numDirectionLights;
uniform int numPointLights;

//uniform vec4 _Colour;
//temp
uniform vec3 cameraPos;
uniform Material material;
vec4 _Colour = vec4(0, 0.03773588, 0.245283, 1);
uniform int wireframe;
uniform samplerCube cubemap;

uniform float _Time;
float _UJump = 0;
float _VJump = 0;
float _Tiling = 10;
float _Speed = 0.5;
float _FlowStrength = 0.1;
float _FlowOffset = 0;
float _HeightScale = 1;
float _HeightScaleModulated = 9;

//function forward declarations
vec3 calcPointLight(PointLight light, vec3 fragNormal, vec3 viewDirection);
vec3 calcSpotLight(SpotLight light, vec3 fragNormal, vec3 viewDirection);
vec3 calcDirectionLight(DirectionLight light, vec3 fragNormal, vec3 viewDirection);
float calcLinearFog();

//allocate space for diffuse texture
vec4 diffTexture;


vec3 FlowUVW(vec2 uv, vec2 flowVector, vec2 jump, float flowOffset, float tiling, float time, bool flowB)
{
    float phaseOffset = flowB ? 0.5 : 0;
    float progress = fract(time + phaseOffset);
    vec3 uvw;
    uvw.xy = uv - flowVector * (progress + flowOffset);
    uvw.xy *= tiling;
    uvw.xy += phaseOffset;
    uvw.xy += (time - progress) * jump;
    uvw.z = 1 - abs(1 - 2 * progress);
    return uvw;
}

vec3 UnpackDerivativeHeight(vec4 textureData)
{
    vec3 dh = textureData.agb;
    dh.xy = dh.xy * 2 - 1;
    return dh;
}

vec4 reflection;
float fogStart = -1.0f;
float fogEnd = 4000;
vec3 fogColor = vec3(0,0,0);


void main()
{
    vec3 flow = texture(textures[1], textureCoord).rgb;
    flow.xy = flow.xy * 2 - 1;
    flow *= _FlowStrength;
    float noise = texture(textures[1], textureCoord).a;
    float time = _Time * _Speed + noise;
    vec2 jump = vec2(_UJump, _VJump);

    vec3 uvwA = FlowUVW(textureCoord, flow.xy, jump, _FlowOffset, _Tiling, time, false);
    vec3 uvwB = FlowUVW(textureCoord, flow.xy, jump, _FlowOffset, _Tiling, time, true);

    float finalHieghtScale = flow.z * _HeightScaleModulated + _HeightScale;
    vec3 dhA = UnpackDerivativeHeight(texture(textures[2], uvwA.xy)) * (uvwA.z * _HeightScale);
    vec3 dhB = UnpackDerivativeHeight(texture(textures[2], uvwB.xy)) * (uvwB.z * _HeightScale);
    vec3 Normal = normalize(vec3(-(dhA.xy + dhB.xy), 1));

    vec4 texA = texture(textures[0], uvwA.xy) * uvwA.z;
    vec4 texB = texture(textures[0], uvwB.xy) * uvwB.z;

	vec4 c = (texA + texB) *_Colour;

	//vec3 norm = normalize(vec3(-(dhA.xy + dhB.xy), 1));
	vec3 norm = normalize(Normal);
	//vec3 norm = normalize(normal);
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 result = vec3(0);

	//add all spotlights
	for (int i = 0; i < min(numSpotLights, MAX_SPOT_LIGHTS); i++)
		result += calcSpotLight(spotLights[i], norm, viewDir);

	//add all point lights
	for (int i = 0; i < min(numPointLights, MAX_POINT_LIGHTS); i++)
		result += calcPointLight(pointLights[i], norm, viewDir);

	//add all direction lights
	for (int i = 0; i < min(numDirectionLights, MAX_DIRECTION_LIGHTS); i++)
		result += calcDirectionLight(directionLights[i], norm, viewDir);

	//add ambient light
	result += ambient_Light;

	//add skybox reflection
	vec3 I = normalize(fragPos - cameraPos);
    vec3 R = reflect(I, normalize(normal + (Normal * 0.1)));
    reflection = vec4(texture(cubemap, R).rgb, 1.0);
	
	//frag_color = reflection;
	//frag_color = (c + vec4(result, 1.0) + vec4(reflection.xyz, 0.5));
	 c = (c + vec4(result, 1.0) + vec4(reflection.xyz, 0.5));
	frag_color = mix(vec4(fogColor,0.0),c,calcLinearFog());

}

float calcLinearFog(){

	float camToPix = length(fragPos - cameraPos);
	float range = fogEnd - fogStart;
	float fogDist = fogEnd - camToPix;
	float FogFactor = fogDist/range;
	FogFactor = clamp(FogFactor,0.0,1.0);
	return 	FogFactor;
}

vec3 calcPointLight(PointLight light, vec3 fragNormal, vec3 viewDirection) {

	vec3 lightDir = normalize(light.position - fragPos);

	vec3 diffuse;
	vec3 specular;

	//diffuse
	float difStrength = max(dot(fragNormal, lightDir), 0.0f);
	diffuse = difStrength * light.diffuse * vec3(texture(material.diffuseTexture, textureCoord));

	//specualar
	vec3 reflectDir = reflect(-lightDir, fragNormal);
	float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.alpha);
	specular = light.specular * vec3(texture(material.specularMap, textureCoord) * spec);

	//attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + distance * light.linear + light.quadratic * (distance * distance));
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = (diffuse + specular);
	return result;
}

vec3 calcSpotLight(SpotLight light, vec3 fragNormal, vec3 viewDirection) {

	vec3 lightDir = normalize(light.position - fragPos);

	vec3 diffuse;
	vec3 specular;


	//spotlight cutoff/feather
	float fragAngle = dot(lightDir, normalize(-light.direction));//angle between spotlights direction and vector from light source to fragment
	if (fragAngle < light.featherAngle)
	{
		diffuse = vec3(0);
		specular = vec3(0);

	}
	else {

		//diffuse
		float difStrength = max(dot(fragNormal, lightDir), 0.0f);
		diffuse = difStrength * light.diffuse * vec3(texture(material.diffuseTexture, textureCoord));

		//specualar
		vec3 reflectDir = reflect(-lightDir, fragNormal);
		float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.alpha);
		specular = light.specular * vec3(texture(material.specularMap, textureCoord) * spec);

		//feather effect
		float e = light.maxAngle - light.featherAngle;
		float intensity = clamp((fragAngle - light.maxAngle) / e, 0.0, 1.0);
		specular *= intensity;
		diffuse *= intensity;
	}

	//emisson map
	//vec3 emission = vec3(texture(material.emissionMap,textureCoord));

	//attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + distance * light.linear + light.quadratic * (distance * distance));
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = (diffuse + specular);
	return result;
}

vec3 calcDirectionLight(DirectionLight light, vec3 fragNormal, vec3 viewDirection) {

	vec3 diffuse;
	vec3 specular;

	//diffuse
	float difStrength = max(dot(fragNormal, light.direction), 0.0f);
	diffuse = difStrength * light.diffuse * vec3(texture(material.diffuseTexture, textureCoord));

	//specualar
	vec3 reflectDir = reflect(-light.direction, fragNormal);
	float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.alpha);
	specular = light.specular * vec3(texture(material.specularMap, textureCoord) * spec);

	vec3 result = (diffuse + specular);
	return result;
}