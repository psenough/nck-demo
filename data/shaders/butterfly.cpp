
#pragma vertex_shader_glx2

void main(){
    gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
}

#pragma fragment_shader_glx2

uniform vec4 gphDiffuseColor;
uniform sampler2D gphTexture0;

void main(){
    vec2 uv = gl_TexCoord[0].xy;
    vec4 texCol = texture2D(gphTexture0, uv);     
    gl_FragColor = vec4(texCol.xyz,1.0);
}


