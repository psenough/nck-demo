
#pragma vertex_shader_glx2

uniform float time;

mat4 RotateZ(float angle){
	return mat4(vec4(cos(angle),sin(angle),0,0),
			vec4(-sin(angle),cos(angle),0,0),
			vec4(0,0,1,0),
			vec4(0,0,0,1.0));
}

void main()
{
	vec3 center = gl_MultiTexCoord1;
	//gl_MultiTexCoord0.x * gl_Normal * 0.01*(time-floor(time/2.0)*2.0);
	
	float ft = (time-floor(time/10.0)*10.0)/10.0;
	float reft = ft;
	ft*=5;
	vec3 p = gl_Vertex.xyz;
	vec3 n = gl_Normal;
	
	mat4 rot = RotateZ(ft*3.1415*4.0);
	vec3 oP = rot * vec4(p-center,0.0);
	
	ft-= gl_MultiTexCoord0.x/51;
	ft = max(ft,0.0);
	ft = min(ft,1.0);
	vec3 pHidden = center-vec3(0,15,0)+vec3(center.x*2.0,0,center.z*2.0);
	
	p = mix(oP+pHidden,p,ft);
	
	p += mix(vec3(0,5,0),vec3(0,0,0),reft); 
	
	
	vec3 nres = mix((rot * vec4(n,0)).xyz,n,ft);
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * vec4(p,1.0);
	gl_FrontColor = gl_Color;
	gl_TexCoord[1].xyz = normalize(gl_NormalMatrix * (nres));
}

#pragma fragment_shader_glx2

void main()
{
	float zn = normalize(gl_TexCoord[1].xyz).z;
	gl_FragColor = vec4(zn,zn,zn,1.0);
}


