#version 330 core

in vec3 TexCoords;//specifies which face of the cube and where in relation to center
uniform samplerCube cubemap;//like sampler 2D buf for cubemaps

out vec4 FragColor;

void main()
{             
    FragColor = texture(cubemap, TexCoords);
}  