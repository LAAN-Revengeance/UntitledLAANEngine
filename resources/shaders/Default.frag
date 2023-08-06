#version 330 core

#define MAX_POINT_LIGHTS 20
#define MAX_SPOT_LIGHTS 20
#define MAX_DIRECTION_LIGHTS 20

struct Material {
    sampler2D diffuseTexture;
    sampler2D specularMap;
    sampler2D emissionMap;
    float alpha;
};

struct PointLight{
	vec3 position;
    vec3 diffuse;
    vec3 specular;

	//attenuation values
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight{
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

struct DirectionLight{
	vec3 direction;
	
	vec3 diffuse;
	vec3 specular;
};

//input values
in vec2 textureCoord;
in vec3 normal;
in vec3 fragPos;

//uniforms
uniform vec3 cameraPos;
uniform Material material;
uniform vec4 color;
uniform int wireframe;
uniform samplerCube cubemap;

uniform vec3 ambient_Light;

uniform SpotLight	   spotLights[MAX_SPOT_LIGHTS];
uniform DirectionLight directionLights[MAX_DIRECTION_LIGHTS];
uniform PointLight	   pointLights[MAX_POINT_LIGHTS];

uniform int numSpotLights;
uniform int numDirectionLights;
uniform int numPointLights;

//output
out vec4 FragColor;

//function forward declarations
vec3 calcPointLight(PointLight light,vec3 fragNormal,vec3 viewDirection);
vec3 calcSpotLight(SpotLight light,vec3 fragNormal,vec3 viewDirection);
vec3 calcDirectionLight(DirectionLight light,vec3 fragNormal,vec3 viewDirection);

vec4 reflection;

void main()
{

	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 result = vec3(0);
	
	//add skybox reflection
	vec3 I = normalize(fragPos - cameraPos);
    vec3 R = reflect(I, normalize(normal));
    reflection = vec4(texture(cubemap, R).rgb, 1.0);
	result += reflection.xyz * vec3(texture(material.specularMap,textureCoord) * material.alpha);

	//add all spotlights
	for(int i = 0; i < min(numSpotLights,MAX_SPOT_LIGHTS);i++)
		result += calcSpotLight(spotLights[i] ,norm ,viewDir);
	
	//add all point lights
	for(int i = 0; i < min(numPointLights,MAX_POINT_LIGHTS);i++)
		result += calcPointLight(pointLights[i] ,norm ,viewDir);
	
	//add all direction lights
	for(int i = 0; i < min(numDirectionLights,MAX_DIRECTION_LIGHTS);i++)
		result += calcDirectionLight(directionLights[i] ,norm ,viewDir);
	
	//add ambient light
	result += ambient_Light;
	
	vec4 diffTexture = texture(material.diffuseTexture,textureCoord);
	
	
	if(wireframe == 0){
		FragColor = (diffTexture * vec4(result,0.0) + (texture(material.emissionMap,textureCoord)));
	}else{
		FragColor =	vec4(1.0,1.0,1.0,1.0);
	}
}


//function implemetnations
vec3 calcPointLight(PointLight light,vec3 fragNormal,vec3 viewDirection){

	vec3 lightDir = normalize(light.position - fragPos);

	vec3 diffuse;
	vec3 specular;

	//diffuse
	float difStrength = max(dot(fragNormal,lightDir),0.0f);
	diffuse = difStrength * light.diffuse * vec3(texture(material.diffuseTexture,textureCoord));
	
	//specualar
	vec3 reflectDir = reflect(-lightDir, fragNormal); 
	float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.alpha);
	specular = light.specular * vec3(texture(material.specularMap,textureCoord) * spec);  
	
	//attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0/(light.constant + distance * light.linear +  light.quadratic  * (distance * distance));
	diffuse  *= attenuation;
	specular *= attenuation;

	vec3 result = (diffuse + specular) ;
	return result;
}

vec3 calcSpotLight(SpotLight light,vec3 fragNormal,vec3 viewDirection){

	vec3 lightDir = normalize(light.position - fragPos);

	vec3 diffuse;
	vec3 specular;
	

	//spotlight cutoff/feather
	float fragAngle = dot(lightDir,normalize(-light.direction));//angle between spotlights direction and vector from light source to fragment
	if(fragAngle < light.featherAngle)
	{
		diffuse = vec3(0);
		specular = vec3(0);

	}else{

		//diffuse
		float difStrength = max(dot(fragNormal,lightDir),0.0f);
		diffuse = difStrength * light.diffuse * vec3(texture(material.diffuseTexture,textureCoord));
	
		//specualar
		vec3 reflectDir = reflect(-lightDir, fragNormal); 
		float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.alpha);
		specular = light.specular * vec3(texture(material.specularMap,textureCoord) * spec);  

		//feather effect
		float e = light.maxAngle - light.featherAngle;
		float intensity = clamp( (fragAngle - light.maxAngle) / e ,0.0 ,1.0 );
		specular *= intensity;
		diffuse *= intensity;
	}

	//emisson map
	//vec3 emission = vec3(texture(material.emissionMap,textureCoord));

	//attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0/(light.constant + distance * light.linear +  light.quadratic  * (distance * distance));
	diffuse  *= attenuation;
	specular *= attenuation;

	vec3 result = (diffuse + specular) ;
	return result;
}

vec3 calcDirectionLight(DirectionLight light,vec3 fragNormal,vec3 viewDirection){

	vec3 diffuse;
	vec3 specular;

	//diffuse
	float difStrength = max(dot(fragNormal,light.direction),0.0f);
	diffuse = difStrength * light.diffuse * vec3(texture(material.diffuseTexture,textureCoord));
	
	//specualar
	vec3 reflectDir = reflect(-light.direction, fragNormal); 
	float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.alpha);
	specular = light.specular * vec3(texture(material.specularMap,textureCoord) * spec);  
	
	vec3 result = (diffuse + specular);
	return result;
}