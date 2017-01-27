
#pragma vertex_shader_glx2

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
}

#pragma fragment_shader_glx2

uniform sampler2D gphTexture0; 
uniform sampler2D gphTexture1; 
uniform sampler2D gphTexture2; 

uniform float time;
uniform vec4 size;

void main()
{
    float tex_frac = 10/256.0;
    float tex_off = 5/256.0;
    
    float t1 = (time - floor(time / 2.0) * 2.0) / 2.0;
    float t2 = (time - floor(time / 5.0) * 5.0) / 5.0;

    vec2 uv = gl_TexCoord[0].xy * vec2(size.x/size.y, size.z / size.y);//*vec2(1.0, 0.5 * size.z / size.y);
    uv += vec2(0.5*(1-size.x/size.y), 0.0);
    
    uv.x += abs(t1-uv.y) < tex_frac ? tex_off : 0.0;
    
    vec3 c1 = texture2D(gphTexture0,uv).xyz;
    vec3 c2 = texture2D(gphTexture1,uv).xyz;
    vec3 c3 = texture2D(gphTexture2,uv).xyz;
     
   
    vec3 c = c1;
    
    if(t2 < 0.3)
       c = c1;
    else if(t2 < 0.6)
       c = c2;
    else
       c = c3;

    c = mix(c,vec3((c.r+c.g+c.b)/3.0),vec3(0.9,0.5,0.9));
    
    float y_step = uv.y * size.y;
    
    if(y_step - floor(y_step / 4.0) * 4.0 < 2.0)
        c = 0.9 * pow(c,vec3(1.1));
    
	gl_FragColor = vec4(c.xyz,0.8);
}


