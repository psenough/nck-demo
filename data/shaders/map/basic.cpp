
#pragma vertex_shader_glx2

void main()
{	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
	gl_TexCoord[1].xyz = normalize(gl_NormalMatrix * (gl_Normal));
}

#pragma fragment_shader_glx2
uniform vec4 gphDiffuseColor;
uniform float gphAlpha;
void main()
{
	float zn = normalize(gl_TexCoord[1].xyz).z;
	gl_FragColor = vec4(gphDiffuseColor.xyz * zn,gphAlpha);
}


