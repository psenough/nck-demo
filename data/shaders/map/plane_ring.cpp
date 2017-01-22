
#pragma vertex_shader_glx2

void main()
{	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
	gl_TexCoord[1].xyz = normalize(gl_NormalMatrix * (gl_Normal));
    gl_TexCoord[0] = gl_MultiTexCoord0;
}

#pragma fragment_shader_glx2
uniform vec4 gphDiffuseColor;
uniform float gphAlpha;
uniform float time;

void main()
{
    float a = gl_TexCoord[0].y;
    
	float zn = normalize(gl_TexCoord[1].xyz).z;
    
    float f = cos(time+2*a*3.1415);
	
    gl_FragColor = vec4(gphDiffuseColor.xyz * abs(cos(time)),clamp(cos(time),0.1,1)*gphAlpha);//gphAlpha*pow(f,3));
}


