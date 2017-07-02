
#pragma vertex_shader_glx2

mat4 RotateY(float angle, vec4 t){
	return mat4(vec4(cos(angle),0,-sin(angle),0),
				vec4(0,1,0,0),
				vec4(sin(angle),0,cos(angle),0),
				t);
}

mat4 RotateX(float angle, vec4 t){
	return mat4(vec4(1,0,0,0),
				vec4(0,cos(angle),sin(angle),0),
				vec4(0,-sin(angle),cos(angle),0),
				t);
}

mat4 RotateZ(float angle, vec4 t){
	return mat4(vec4(cos(angle),sin(angle),0,0),
			vec4(-sin(angle),cos(angle),0,0),
			vec4(0,0,1,0),
			t);
}

uniform vec4 distance;
uniform vec4 modifier;

void main()
{
	float yWorldPos = gl_Vertex.y + distance.y;
	float alpha = (yWorldPos-distance.z) * 0.0006 * modifier.x;
	float zeta = sin((yWorldPos-distance.z) * 0.002 * modifier.y);
	
	mat4 rY = RotateY(zeta*10,vec4(0,distance.y-distance.z,0,1));
	mat4 rX = RotateX(zeta,vec4(0,0,0,1));
	mat4 rZ = RotateZ(zeta,vec4(0,0,0,1));
	mat4 rRes = rZ * rX * rY ;
	
	gl_TexCoord[0] = vec4(gl_MultiTexCoord0.x,gl_MultiTexCoord0.y,yWorldPos-distance.z,0);
	gl_Position = gl_ModelViewProjectionMatrix * ( rRes * gl_Vertex );
	gl_FrontColor = gl_Color;
	gl_TexCoord[1].xyz = gl_Normal;
}

#pragma fragment_shader_glx2

uniform sampler2D gphTexture0;

void main()
{
	//float diff =normalize().z;
	float amb = 0.4;
	
	float depth = abs(1 - gl_TexCoord[0].z / 500.0); 
	depth *= depth;

	gl_FragColor = vec4(gl_TexCoord[1].xyz,1.0);//texture2D(gphTexture0, gl_TexCoord[0].xy).xyz*colDiff*depth,1.0);
}


