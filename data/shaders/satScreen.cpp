
#pragma vertex_shader_glx2

varying vec2 uv;
varying vec3 pos;

void main()
{
    pos = gl_Vertex.xyz;
    uv = gl_MultiTexCoord0.xy;
    gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
}

#pragma fragment_shader_glx2

uniform sampler2D gphTexture0;
varying vec2 uv;
varying vec3 pos;


void main()
{
    vec2 uvv = vec2(uv.x,-uv.y);
    
    vec3 col = texture2D(gphTexture0,uvv).xyz;
    
    gl_FragColor = vec4(col,1.0);//mix(vec3(gl_TexCoord[0].x,gl_TexCoord[0].y,0.0),texture2D(gphTexture0,gl_TexCoord[0].xy).xyz,0.5),1.0);
}


