#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTexture;

out vec2 textureCoord;
out vec3 normal;
out vec3 fragPos;


uniform mat4 model;			// model matrix
uniform mat4 view;			// view matrix
uniform mat4 projection;	// projection matrix
uniform float _Time;


vec4 _WaveA = vec4(1, 1, 0.25, 60);
vec4 _WaveB = vec4(0, 0.6, 0.25, 31);
vec4 _WaveC = vec4(1, 1.3, 0.13, 18);


const float PI = 3.1415926535897932384626433832795;

vec3 GersterWave(vec4 wave, vec3 p, inout vec3 tangent, inout vec3 binormal)
{
	float steepness = wave.z;
	float wavelength = wave.w;
	//wavelength
	float k = 2 * PI / wavelength;
	//phase speed 
	float c = sqrt(9.8 / k);
	//direction
	vec2 d = normalize(wave.xz);
	//frequnce
	float f = k * (dot(d, p.xz) - c * _Time);
	//amplitude
	float a = steepness / k;

	tangent += vec3(-d.x * d.x * (steepness * sin(f)), d.x * (steepness * cos(f)), -d.x * d.y * (steepness * sin(f)));
	binormal += vec3(-d.x * d.y * (steepness * sin(f)), d.y * (steepness * cos(f)), -d.y * d.y * (steepness * sin(f)));

	return vec3(d.x * (a * cos(f)), a * sin(f), d.y * (a * cos(f)));
}

void main()
{
	vec3 gridPoint = aPos;
	vec3 tangent = vec3(1, 0, 0);
	vec3 binormal = vec3(0, 0, 1);
	//final position
	vec3 p = gridPoint;
	p += GersterWave(_WaveA, gridPoint, tangent, binormal);
	p += GersterWave(_WaveB, gridPoint, tangent, binormal);
	p += GersterWave(_WaveC, gridPoint, tangent, binormal);
	vec3 Normal = normalize(cross(binormal, tangent));

	fragPos = vec3(model * vec4(p, 1.0));
	textureCoord = aTexture;
	normal = mat3(transpose(inverse(model))) * aNorm;
	gl_Position = projection * view * model * vec4(p, 1.0);

}