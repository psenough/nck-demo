
#pragma vertex_shader_glx2


void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
}

#pragma fragment_shader_glx2
uniform sampler2D tex1;
void main()
{
	vec3 col = texture2D(tex1,gl_TexCoord[0].xy).xyz;
	gl_FragColor = vec4(col,1.0);
}


