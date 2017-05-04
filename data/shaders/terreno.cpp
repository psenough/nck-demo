
#pragma vertex_shader_glx2

varying float z;

void main()
{
    z = gl_Vertex.z;
    gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
}

#pragma fragment_shader_glx2

varying float z;

void main()
{
    
    float s = z * 100.0;
    s = s - floor(s/2.0) * 2.0;
    if(s < 0.1)
        gl_FragColor = vec4(vec3(1.0,1.0,1.0),1.0);
    else
        discard;
}


