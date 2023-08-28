#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
	float depthVal = texture(screenTexture, TexCoords).r;
	//FragColor = vec4(vec3(depthVal),1.0);
	FragColor = texture(screenTexture, TexCoords);
}