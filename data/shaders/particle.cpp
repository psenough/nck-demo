
#pragma vertex_shader_glx2

#define FFT_TEX_SIZE 128

uniform float dt;
uniform float flip;
uniform sampler2D gphTexture0;

varying vec3 vNor;

mat4 RotateZ(float angle){
	return mat4(vec4(cos(angle),sin(angle),0,0),
			vec4(-sin(angle),cos(angle),0,0),
			vec4(0,0,1,0),
			vec4(0,0,0,1.0));
}


void main()
{
    vec3 P = gl_Vertex.xyz;
    vec2 UV = gl_MultiTexCoord0.xy;
    vNor = P;
    
    
    float value = texture2D (gphTexture0,abs((UV-vec2(0.5,0.0))*vec2(4.0,1.0))).r;
    float lowFValue = 
            texture2D (gphTexture0,vec2(0.0,0.0)).r+
             texture2D (gphTexture0,vec2(1.0/FFT_TEX_SIZE,0)).r+
               texture2D (gphTexture0,vec2(2.0/FFT_TEX_SIZE,0)).r+
                 texture2D (gphTexture0,vec2(3.0/FFT_TEX_SIZE,0)).r;
    
    lowFValue /= 4.0;    
    lowFValue = abs(lowFValue);
 
    vec2 PP = mix(vec2(1.0,1.0), vec2(0.25,1.0),UV.y) * (UV - vec2(0.5)) * 64.0 ;
 
 
    float y = mix(0.025,0.01,UV.y) * value * flip * (lowFValue*0.05) +
    + cos(UV.y*5.0+dt*0.1);
 
    float size = mix(0.01,0.1,UV.y);
    
    vec3 final_position = P * size + vec3(PP.x, y, PP.y);
    final_position.z -= FFT_TEX_SIZE*0.25 + mix(FFT_TEX_SIZE,0,clamp(dt / 20,0,1));
    
	//+cos(UV.y*1.2+dt*0.1)  * *
    vec4 fp =   RotateZ(0.01 * max(dt-10.0,0.0)) * vec4(final_position,1.0);
    
    gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ProjectionMatrix * fp;
	gl_FrontColor = gl_Color;
}

#pragma fragment_shader_glx2

varying vec3 vNor;
uniform float dt;


void main()
{
    vec2 UV = gl_TexCoord[0].xy;
    float depth = UV.y;
    float side = abs(UV.x - 0.5)*2.0;
    
    float cD = length(vNor*0.5);

    vec3 color1 = mix(vec3(1.0,0.8,0.0),vec3(0.0,0.8,1.0),UV.x);
    vec3 color2 = mix(vec3(1.0,0.0,0.0),vec3(1.0,0.8,0.0),UV.x);
    
    float colorTSpan = 10.0;
    float colorAlpha = (dt - floor(dt/colorTSpan)*colorTSpan);
    
    if(colorAlpha > colorTSpan * 0.5)
        colorAlpha = colorTSpan - colorAlpha;
    
    vec3 colorFinal = mix(color1,color2,2*colorAlpha/colorTSpan);
    
    float simple_ball = mix(1,0,clamp(cD*2.0,0,1));
    float dull_ball = pow(mix(1,0,clamp(cD*2.0,0,1)),0.2);
    float dull_ball2 = pow(mix(1,0,clamp(cD*2.5,0,1)),0.2);
    
    float alpha = mix(0.5 * dull_ball * (1-dull_ball2),simple_ball,min(depth,side)) ;
    
    alpha = alpha * mix(1.0,0.2,depth);
	gl_FragColor = vec4(colorFinal, clamp(max(dt-1.0,0.0)/10.0,0,1) * alpha);
}


