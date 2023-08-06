



//#version 330 core
//
//struct Material {
//    sampler2D diffuseTexture;
//    sampler2D specularMap;
//    sampler2D emissionMap;
//    float alpha;
//};
//
//uniform Material material;
//
//vec4 m_MainColor = vec4(1,0,1,1);
//uniform float _Time;
//
//
//float m_BarSpeed = 1;
//float m_BarDistance =100;
//float m_Alpha;
//float m_FlickerSpeed = 0.5;
//float m_GlowSpeed = 100;
//float m_GlowDistance = 100;
//
//
////input values
//in vec2 textureCoord;
//in vec3 normal;
//in vec3 fragPos;
//
////thanks, github
//float rand(float n){
//    return fract(sin(n) * 43758.5453123);
//}
//
//float noise(float p){
//    float fl = floor(p);
//    float fc = fract(p);
//    return mix(rand(fl), rand(fl + 1.0), fc);
//}
//
//void main(){
//    
//    //Texture
//    vec4 texColor = vec4(1.0);
//    texColor = texture2D(material.diffuseTexture, textureCoord);
//
//    //Scan effect
//    float bars = 0.0;
//    float val = _Time * m_BarSpeed + fragPos.y * m_BarDistance;
//    bars = step(val - floor(val), 0.5) * 0.65;
//
//    //Just plain old alpha
//    float alpha = 1.0;
//    #ifdef HASALPHA
//        alpha = m_Alpha;
//    #endif
//
//    //Flickering
//    float flicker = 1.0;
//    flicker = clamp(noise(_Time * m_FlickerSpeed), 0.65, 1.0);
//
//    //Rim lighting
//    float rim = 1.0;
//    vec4 rimColor = vec4(0.0);
//
//
//    //Glow
//    float glow = 0.0;
//    float tempGlow = fragPos.y * m_GlowDistance - _Time * m_GlowSpeed;
//    glow = tempGlow - floor(tempGlow);
//    
//    vec4 color = texColor * m_MainColor + rimColor + (glow * 0.35 * m_MainColor);
//    color.a = texColor.a * alpha * (bars + rim + glow) * flicker;
//    gl_FragColor = color;
//}












/////////////////////////////
#version 330 core

//input values
in vec2 textureCoord;
in vec3 normal;
in vec3 fragPos;

//output
out vec4 FragColor;

//uniforms
vec4 color = vec4(0.8,0,1,1);
uniform float _Time;

float random (in vec2 st);
float noise (in vec2 st);

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

    //scanlines
    float y = 0;
    for(int i = 0; i < 40; i++){
    float tile = sin(textureCoord.y + (_Time + (i * 2)) * 0.1);
	    y += cubicPulse(0.5,0.005, tile);
    }
    vec4 scanCol = vec4(y, 0, y, y);



    //noise
    float offset  = -_Time * 0.1;//change mult for animation speed
    vec2 res = vec2(8, 8);
    vec2 st = vec2(textureCoord.x,textureCoord.y + offset)/res;
	vec2 pos = vec2(st*100);//mult for scale
	float n = noise(pos);
   
    vec4 aNoise  = vec4(1,1,1,1) * n/2;
    FragColor = vec4(0.7,0,1,0.1) + (color * aNoise) + scanCol;
}

// psudo random number based on 2D seed
float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
}


float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = smoothstep(0.,1.,f);

    return mix(a, b, u.x) + (c - a)* u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}
