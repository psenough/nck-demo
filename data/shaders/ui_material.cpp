
#pragma vertex_shader_glx2

void main()
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
}

#pragma fragment_shader_glx2

uniform vec4 gphDiffuseColor;
 
void main()
{
    
  
    gl_FragColor = vec4(gphDiffuseColor.xyz,1.0);

}


