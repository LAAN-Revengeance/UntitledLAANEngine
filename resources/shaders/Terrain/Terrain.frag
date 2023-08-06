#version 330 core

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

uniform vec3 cameraPos;
uniform Material material;
uniform vec4 color;
uniform int wireframe;


//input values
in vec2 textureCoord;
in vec3 normal;
in vec3 fragPos;

//light uniforms
uniform vec3 ambient_Light;
uniform SpotLight	   spotLights[MAX_SPOT_LIGHTS];
uniform DirectionLight directionLights[MAX_DIRECTION_LIGHTS];
uniform PointLight	   pointLights[MAX_POINT_LIGHTS];

uniform int numSpotLights;
uniform int numDirectionLights;
uniform int numPointLights;

//terrain texture data
uniform int textureCount;
uniform sampler2D textures[MAX_TEXTURES];

uniform float heights[MAX_TEXTURES];
uniform int heightCount;

//output
out vec4 FragColor;

//function forward declarations
vec3 calcPointLight(PointLight light,vec3 fragNormal,vec3 viewDirection);
vec3 calcSpotLight(SpotLight light,vec3 fragNormal,vec3 viewDirection);
vec3 calcDirectionLight(DirectionLight light,vec3 fragNormal,vec3 viewDirection);
vec4 CalcTextureMix();

//allocate space for diffuse texture
vec4 diffTexture;

void main()
{
	
	diffTexture = CalcTextureMix();

	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 result = vec3(0);
	
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

	
	if(wireframe == 0){
		FragColor = (diffTexture * vec4(result,1.0)) + ((texture(material.emissionMap,textureCoord) * 1.5));
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
		diffuse = difStrength * light.diffuse * vec3(diffuse);
	
		//specualar
		vec3 reflectDir = reflect(-lightDir, fragNormal); 
		float spec = pow(max(dot(viewDirection, reflectDir), 0.0), alpha);
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
	
	vec3 result = (diffuse + specular) ;
	return result;
}

vec4 CalcTextureMix(){

    vec4 result = vec4(0);
    float height = fragPos.y;


	vec4 colors[MAX_TEXTURES];
	colors[0] = texture(textures[0],textureCoord);
	for(int i = 1; i <= textureCount + 1; i++)
		colors[i] = texture(textures[i-1],textureCoord);


    if(height < heights[0]){
        result = texture(textures[0],textureCoord);
    }else if(height <= heights[heightCount - 1])
	{
        for(int i = 0; i < textureCount + 1; i++)
		{
            if(height <= heights[i + 1] && height >= heights[i])
			{
                float factor = ((height - heights[i])/(heights[i + 1] - heights[i]));
				factor = smoothstep(0.0, 1.0, factor);
                result = (mix(colors[i],colors[i + 1],factor));
                break;
            }
         }

    }else{
        result = colors[textureCount];
    }

	//add detail map
	result += colors[textureCount + 1];

    return result;
}