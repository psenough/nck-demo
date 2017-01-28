
#pragma vertex_shader_glx2

uniform float time;
uniform float delay;
uniform float failure;
uniform float rotate;

mat4 RotateZ(float angle){
	return mat4(vec4(cos(angle),sin(angle),0,0),
			vec4(-sin(angle),cos(angle),0,0),
			vec4(0,0,1,0),
			vec4(0,0,0,1.0));
}

void main()
{
	vec3 center = gl_MultiTexCoord1.xyz;
	vec3 random = gl_MultiTexCoord2.xyz;
    
	float ft = time/delay;
	float reft = ft;
	ft*=15;
	vec3 p = gl_Vertex.xyz;
	vec3 n = gl_Normal;
	
    mat4 rot1 = RotateZ(rotate);
    p = rot1 * vec4(p,0.0);
    
	mat4 rot = RotateZ(time*3.1415*4.0);
	vec3 oP = rot * vec4(p-center,0.0);
	
	ft-= gl_MultiTexCoord0.x/184;
	ft = clamp(ft,0.0,1.0);
  
	vec3 pHidden = center-vec3(0,10,0)+vec3(4.0*(center.x>0.0?1.0:-1.0),0,2.0*(center.z>0.0?1.0:-1.0));
	
    vec3 nres = mix((rot * vec4(n,0)).xyz,n,ft);
	
    p = mix(oP+pHidden,p,ft);
    p += mix(vec3(0,5,0),vec3(0,2,0),reft);
     
    if(time > 9.0 && failure >0.5){
        if(ft > 0.9){
            float dt = time - 9.0;
            p = p + random * (1.0 * dt - 0.5 * 9.8 * dt * dt);
        }else{
            p = pHidden;
        }
    }
	
    
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * vec4(p,1.0);
	gl_FrontColor = gl_Color;
	gl_TexCoord[1].xyz = normalize(gl_NormalMatrix * (nres));
}

#pragma fragment_shader_glx2

void main()
{
	//float zn = normalize(gl_TexCoord[1].xyz).z;
	gl_FragColor = vec4(0,0,0,1.0);
}


