
#pragma vertex_shader_glx2

void main()
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
}

#pragma fragment_shader_glx2

uniform float dt;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    
    /*vec3 color1 = mix(vec3(1.0,0.8,0.0),vec3(0.0,0.8,1.0),0.5);
    vec3 color2 = mix(vec3(1.0,0.0,0.0),vec3(1.0,0.8,0.0),0.5);
    
    float colorTSpan = 10.0;
    float colorAlpha = (dt - floor((dt)/colorTSpan)*colorTSpan);
    
    if(colorAlpha > colorTSpan * 0.5)
        colorAlpha = colorTSpan - colorAlpha;
    
        
    vec3 colorFinal = mix(color1,color2,2*colorAlpha/colorTSpan);
    */


    float len = 30.0;
    float factor = len * 2.0 * (dt - floor(dt/9.0)*9.0)/9.0;//30 * (dt - floor(dt/9)*9)/9;
    
    
    //
    vec3 colorFinal = mix(vec3(1.0,0.2,0.0),vec3(0.4,0.0,0.0),uv.x);
        
    float hB = uv.x * 3.0 - floor(uv.x * 3.0);
    if( hB > 0.8 || hB < 0.2)
        hB = 0.5;
    else
        hB = 1.0;
    
    float vB = uv.y*2 - floor(uv.y*2);
    if( vB > 0.8 || vB < 0.1)
        vB = 0.5;
    else
        vB = 1.0;
      
    float a = hB * vB;
    
    if(factor + floor(uv.y) - floor(uv.x * 3.0) * 4.0 < 0)
        a = 0.0;
    else if(factor > len && 
        factor - len + floor(uv.y) - floor(uv.x * 3.0) * 4.0 > 0)
        a = 0.0;
   
	gl_FragColor = vec4(colorFinal,a);
}


