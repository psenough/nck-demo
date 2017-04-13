
#pragma vertex_shader_glx2

void main()
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
}

#pragma fragment_shader_glx2

uniform sampler2D gphTexture0;

void main()
{
  
    vec2 uv = gl_TexCoord[0].xy;
    vec2 off = uv * 0.5 - 0.5;
    vec2 nor = normalize(off);
    float dist = length(off)*0.1;
    
    float r = texture2D (gphTexture0, uv - off * dist* 0.99).r;
    float g = texture2D (gphTexture0, uv - off * dist* 0.85).g;
    float b = texture2D (gphTexture0, uv - off * dist* 0.90).b;
   
    vec3 col = vec3(r,g,b) * mix(1.0,2.0,dist*10.0);
    float dY = pow(1.0  - abs(uv.y - 0.5)*2.0,0.2);

  
	gl_FragColor = vec4(vec3(dY*col),1.0);
}


