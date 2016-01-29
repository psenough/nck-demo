
#pragma vertex_shader_glx2
void main(){
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
}
#pragma fragment_shader_glx2
uniform vec4 color;

void main()
{
	vec2 pos = gl_TexCoord[0].xy;
	vec2 v = (pos * 2.0 - vec2(1.0));
	float l = pow(1-length(v),2.0);
	gl_FragColor = vec4(color.xyz,l);
}