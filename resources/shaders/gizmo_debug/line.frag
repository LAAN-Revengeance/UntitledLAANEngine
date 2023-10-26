#version 330 core


in vec3 fragPos;

uniform vec4 color;

//output
out vec4 FragColor;

void main()
{
	//FragColor =	vec4(1.0,1.0,1.0,1.0);
	FragColor =	color;
}