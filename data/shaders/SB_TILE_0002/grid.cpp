
#pragma vertex_shader_glx2
varying vec3 position;

void main()
{
	vec4 p = gl_Vertex;
	position = p.xyz;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * p;
}

#pragma fragment_shader_glx2

varying vec3 position;
uniform float time;

// +vec3(0,time*50.0,0)
float samplePos(vec2 p, vec2 uv, vec2 delta){
	vec2 pos = p * uv + delta;
	vec2 dp = floor(pos/10.0)*10.0;
	vec2 off = abs(pos-dp.xy);
	
	if(off.x < 0.8 || off.y < 0.8)
		return 0.0;
	return 1.0;
}

float sampleGrid(vec2 p, vec2 uv, vec2 delta){
	float dist = length(p);
	float o = dist / 1000.0;
	
	float r = samplePos(p,uv,delta+vec2(0.0));
	r += samplePos(p,uv,delta+vec2(-o,0.0));
	r += samplePos(p,uv,delta+vec2(o,0.0));
	r += samplePos(p,uv,delta+vec2(0.0,o));
	r += samplePos(p,uv,delta+vec2(0.0,-o));
	r += samplePos(p,uv,delta+vec2(-o,-o));
	r += samplePos(p,uv,delta+vec2(o,o));
	r += samplePos(p,uv,delta+vec2(-o,o));
	r += samplePos(p,uv,delta+vec2(o,-o));
	
	return r / 9.0;
}

void main()
{	
	vec4 pinkish = vec4(1.0,0.2,0.5,1.0);
	
	vec2 uv = gl_TexCoord[0].xy;
	float v = sampleGrid(position.xy,uv,vec2(0)+vec2(0,time*50.0));
	
	float d = clamp(length(position.xy) / 1000.0,0,1);
	
	float fog = (1.0 - d*6.0);
	
	vec4 col = vec4(mix(pinkish.xyz,vec3(0),(fog*v)*0.8),1);
		
	gl_FragColor = col;
}
