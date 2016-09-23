
#pragma vertex_shader_glx2

uniform float time;

void main()
{
	vec3 n = gl_MultiTexCoord0.x * gl_Normal * 0.01*(time-floor(time/2.0)*2.0);
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex+vec4(n,0.0));
	gl_FrontColor = gl_Color;
	gl_TexCoord[1].xyz = normalize(gl_NormalMatrix*gl_Normal);
}

#pragma fragment_shader_glx2

void main()
{
	float zn = normalize(gl_TexCoord[1].xyz).z;
	gl_FragColor = vec4(zn,zn,zn,1.0);
}


