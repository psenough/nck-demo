
#pragma vertex_shader_glx2

#define FFT_TEX_SIZE 128

uniform float dt;
uniform sampler2D gphTexture0;

varying vec3 vNor;
varying float z;

void main()
{
    vec3 P = gl_Vertex.xyz;
    vec2 UV = gl_MultiTexCoord0.xy;
    vNor = P;
    
    float value = texture2D (gphTexture0,UV).r;
 
    vec2 PP = mix(vec2(1.0,1.0), vec2(0.25,1.0),UV.y) * (UV - vec2(0.5)) * 64.0 ;
 
 
    float y = value* 20.0;
     
    float size = 0.02;
    
    vec3 FP = P * size + vec3(PP.x, y, PP.y);
    
    FP.z += dt-64 -floor(dt/80.0)*80.0;
  
    z = FP.z;
    
    vec4 fp = vec4(FP,1.0);
    
    gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ProjectionMatrix * fp;
	gl_FrontColor = gl_Color;
}

#pragma fragment_shader_glx2

varying vec3 vNor;
uniform float dt;
varying float z;

void main()
{
    vec2 UV = gl_TexCoord[0].xy;
    float depth = UV.y;
    float side = abs(UV.x - 0.5)*2.0;
    float zDepth = clamp(-z / 64.0,0,1);
    
    float cD = length(vNor*0.5);

    float simple_ball = mix(1,0,clamp(cD*2.0,0,1));
    float dull_ball = pow(simple_ball,0.2);
    
    float alpha = dull_ball ;
    
	gl_FragColor = vec4(vec3(1.0), sin(zDepth*2*3.1415) * 0.5*alpha);
}


