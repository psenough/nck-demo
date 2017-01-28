
#pragma vertex_shader_glx2


void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
}

#pragma fragment_shader_glx2
uniform sampler2D tex1;
uniform float time;

vec3 desat(vec3 col){
    return vec3((col.r+col.g+col.b)/3.0);
}

void main()
{
	vec3 col = texture2D(tex1,gl_TexCoord[0].xy).xyz;
   
    float t = (time - floor(time / 10.0) * 10.)/10.0;
    float f = 1.0/512;
    col = desat(col);
    
  
     
    col = (pow(col,vec3(4.0)) * 0.2 + 0.6)*0.25;  
      
        float dx =  gl_TexCoord[0].x * 128;
    float dy =  gl_TexCoord[0].y * 128;
    if((dx - floor(dx/16.0)*16.0 < 0.125) || 
        (dy - floor(dy/16.0)*16.0 < 0.125))
        col *= 1.5
       ;
        
	gl_FragColor = vec4(col,1.0);
}


