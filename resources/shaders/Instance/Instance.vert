#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexture;
layout (location = 3) in mat4 instanceMat;

out vec2 textureCoord;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{

	fragPos = vec3(instanceMat * vec4(aPos, 1.0));
	textureCoord = aTexture;
	normal = aNorm;
	gl_Position =  projection * view *  instanceMat * vec4(aPos, 1.0);
}