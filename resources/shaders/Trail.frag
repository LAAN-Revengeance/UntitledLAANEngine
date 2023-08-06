#version 330 core
out vec4 FragColor;

//input values
in vec2 textureCoord;
in vec3 normal;
in vec3 fragPos;

uniform vec4 color;//Trail Colour



uniform float time; //GLFWgetTime()
uniform float scale; // hex Scale
uniform float offset; // Hex Offset


const vec2 s = vec2(1,1.7320508);


float cubicPulse( float c, float w, float x )
{
    x = abs(x - c);
    if( x > w ) return 0.0;
    x /= w;
    return 1.0 - x * x *(3.0 - 2.0 * x);
}

void main()
{    
    vec3 colour = vec3(color.x, color.y, color.z);
    

	float tile = sin(textureCoord.y * 0.75);

	float y = cubicPulse(0.5,0.2, textureCoord.y/50);

    vec4 texColour = vec4(y, 0, 0, y);
       
    FragColor = texColour;
}
