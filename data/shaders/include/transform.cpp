
#ifdef VSH

uniform vec4 tr_distance;
uniform vec4 tr_modifier;

mat4 RotateY(float angle, vec4 t){
	return mat4(vec4(cos(angle),0.0,-sin(angle),0.0),
				vec4(0.0,1.0,0.0,0.0),
				vec4(sin(angle),0.0,cos(angle),0.0),
				t);
}

mat4 RotateX(float angle, vec4 t){
	return mat4(vec4(1.0,0.0,0.0,0.0),
				vec4(0.0,cos(angle),sin(angle),0.0),
				vec4(0.0,-sin(angle),cos(angle),0.0),
				t);
}

mat4 RotateZ(float angle, vec4 t){
	return mat4(vec4(cos(angle),sin(angle),0.0,0.0),
			vec4(-sin(angle),cos(angle),0.0,0.0),
			vec4(0.0,0.0,1.0,0.0),
			t);
}

void transform_project_and_twist(vec4 P){
    vec3 pMM = (gphModelMatrix * P).xyz;
    
    pMM += vec3(0.0,tr_distance.y-tr_distance.z,0.0);
    
    float theta = pMM.y+tr_distance.x;// camera offset in y axis 
    
    mat4 rX = RotateX(theta*0.006* tr_modifier.x,vec4(0,0,0,1));
    mat4 rY = RotateY(theta*0.006* tr_modifier.y,vec4(0,0,0,1));
    mat4 rZ = RotateZ(theta*0.01* tr_modifier.z,vec4(0,0,0,1));
    
    mat4 rTotal = rX * rY * rZ;
    
    gl_Position = (gphProjectionMatrix * gphViewMatrix) * (rTotal * vec4(pMM,1.0));
}

#endif
