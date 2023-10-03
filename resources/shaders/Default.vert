#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexture;
layout (location = 2) in vec3 aTangent;
layout (location = 2) in vec3 aBiTanget;

out vec2 textureCoord;
out vec3 normal;
out vec3 fragPos;
out vec4 fragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
	fragPos = vec3(model * vec4(aPos, 1.0));
	textureCoord = aTexture;
	normal = mat3(transpose(inverse(model))) * aNorm;
	fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);

	gl_Position =  projection * view *  model * vec4(aPos, 1.0);
}