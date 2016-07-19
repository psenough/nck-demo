
#pragma vertex_shader_glx2

uniform float time;

mat4 RotateZ(float angle){
	return mat4(vec4(cos(angle),sin(angle),0,0),
			vec4(-sin(angle),cos(angle),0,0),
			vec4(0,0,1,0),
			vec4(0,0,0,1.0));
}

float limit_value(float v, float repeat){
	return v - floor(v/repeat)*repeat;
}

vec3 offFunc(vec3 p,float t){
	return vec3(p.x,p.y + 1.0 * cos(p.z + t*10.0),p.z);
}

varying vec4 vPos, vOPos;
varying vec3 vNor;
varying float timeRef;

void main()
{
	vec4 p = gl_Vertex;
	vec3 n = gl_Normal;
	
	float t = limit_value(time * 0.0001, 3.1415 * 4.0);
	//mat4 rMat = RotateZ(t);
	
	float direction = cos(t / 2.0);
	float blend = direction*(cos(p.z));
	//float stretch = clamp(cos(p.z-2.0),0.0,1.0);
	mat4 rMat = RotateZ(direction*(t+cos(p.z)));
	
	p *= vec4(1.0,0.1,2.0,1.0);
	
	timeRef = t;
	vOPos = p;
	
	p = vec4(offFunc(p,t),1.0);
	n = vec4(n,0.0);
	
	vPos = p;
	vNor = n;
	
	//vec3 ref = vPos;
	vec3 cr = cross(vNor,vec3(0,0,-1));
	vec3 s0 = offFunc(vOPos+vec3(0,0,0.01),timeRef);
	vec3 nn = normalize(cross(vPos - s0,cr));
	
	n = nn;
	
	gl_Position = gl_ModelViewProjectionMatrix * rMat * p;
	gl_FrontColor = gl_Color;
	gl_TexCoord[1].xyz = gl_NormalMatrix * (rMat * vec4(n,0.0)).xyz;
}

#pragma fragment_shader_glx2

uniform sampler2D tex1;
varying float timeRef;

vec3 offFunc(vec3 p,float t){
	return vec3(p.x, p.y + 0.5 * cos(p.z + t*10.0), p.z);
}

varying vec4 vPos,vOPos;
varying vec3 vNor;

void main()
{	
	float zn = normalize(gl_TexCoord[1].xyz).z;
	gl_FragColor = vec4(vec3(zn),1.0);
}


