
#pragma vertex_shader_glx2

void main()
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
}

#pragma fragment_shader_glx2

uniform sampler2D gphTexture0;
uniform float alpha;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;    
	gl_FragColor = vec4(texture2D (gphTexture0, uv).xyz,alpha/255.0);
}


