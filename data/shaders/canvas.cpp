
#pragma vertex_shader_glx2
void main(){
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
}
#pragma fragment_shader_glx2
uniform sampler2D texture_0;
void main()
{
	vec2 uv = gl_TexCoord[0].xy;
	vec2 v = (uv * 2.0 - vec2(1.0));
		
	vec4 col1 = texture2D(texture_0,uv - 0.1 * v * length(v) ); // *pow(1.0+v,2.0)
	vec4 col2 = pow(col1 * 0.8,2.0);
	

	float l = 1.0-pow(length(v)*0.75,2.0);
	vec4 col = mix(col2,col1,l);
	gl_FragColor = col;
}