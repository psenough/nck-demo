
#pragma vertex_shader_glx2

varying vec3 pMV;

void main()
{	
    pMV = (gl_ModelViewMatrix * gl_Vertex).xyz;
    
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
	gl_TexCoord[1].xyz = normalize(gl_NormalMatrix * (gl_Normal));
}

#pragma fragment_shader_glx2
varying vec3 pMV;

void main()
{
	float zn = clamp(0.8/(length(pMV)),0,1);
    zn = clamp(pow(zn,3),0,1);
	gl_FragColor = vec4(zn,zn,zn,1.0);
}


