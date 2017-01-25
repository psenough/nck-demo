
#pragma vertex_shader_glx2

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
}

#pragma fragment_shader_glx2

uniform sampler2D gphTexture0; 
uniform float time;
uniform vec4 size;

void main()
{
    float t = time - floor(time); // [0,1]

    vec2 uv = gl_TexCoord[0].xy*vec2(0.5, 0.5 * size.z / size.y);
    uv += vec2(0.0, t);
    
    if(uv.y > 1.0)
        uv = vec2(uv.x+0.5, uv.y - 1.0);
    
    vec4 c = texture2D(gphTexture0,uv).xyzw;
	gl_FragColor = vec4(vec3(1.0)-c.xyz,0.8);
}


