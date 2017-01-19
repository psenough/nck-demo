
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
    return (col.r+col.g+col.b)/3.0;
}
void main()
{
	vec3 col = texture2D(tex1,gl_TexCoord[0].xy).xyz;
   
    float t = (time - floor(time / 10.0) * 10.)/10.0;
    float x = gl_TexCoord[0].x;
    float f = 1.0/512;
    col = desat(col);
    if(x > t && x < t+f || 
            x-f*4.0 > t && x - f * 4.0 < t+f*0.5 || 
             x+f*4.0 > t && x + f * 4.0 < t+f*0.5|| 
            x-f*8.0 > t && x - f * 8.0 < t+f*0.25 || 
             x+f*8.0 > t && x + f * 8.0 < t+f*0.25
             
             ){
       col *= 1.4; 
    }
    
     
    col = pow(col,vec3(4.0)) * 0.4 + 0.4;  
    col *= vec3(0.9,0.95,1.0)*0.5;
    
	gl_FragColor = vec4(col,1.0);
}


