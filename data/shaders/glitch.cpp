
/**
 * Glitch Effect Post-Process
 * Found on shadertoy by Kusma
 * https://www.shadertoy.com/view/Md2GDw
 */

#pragma fragment_shader_glx2

uniform sampler2D gphTexture0;
uniform sampler2D gphTexture1;
uniform vec4 texResolution;
uniform float time;

void process(out vec4 fragColor, vec2 fragCoord, vec2 iResolution, float iGlobalTime )
{
	vec2 uv = fragCoord.xy / iResolution.xy;
	vec2 block = floor(fragCoord.xy / vec2(16.0));
	vec2 uv_noise = block / vec2(64.0);
	uv_noise += floor(vec2(iGlobalTime) * vec2(1234.0, 3543.0)) / vec2(64.0);
	
	float block_thresh = pow(fract(iGlobalTime * 1236.0453), 2.0) * 0.1;
	float line_thresh = pow(fract(iGlobalTime * 2236.0453), 3.0) * 0.1;
	
	vec2 uv_r = uv, uv_g = uv, uv_b = uv;

	// glitch some blocks and lines
	if (texture2D(gphTexture1, uv_noise).r < block_thresh ||
		texture2D(gphTexture1, vec2(uv_noise.y, 0.0)).g < line_thresh) {

		vec2 dist = (fract(uv_noise) - 0.5) * 0.3;
		uv_r += dist * 0.1;
		uv_g += dist * 0.2;
		uv_b += dist * 0.125;
        
        if(fragCoord.y < iResolution.y * 0.5){
            uv_r.x += dist.x;
            uv_g.x += dist.x;
            uv_b.x += dist.x;
        }
	}     
        
	fragColor.r = texture2D(gphTexture0, uv_r).r;
	fragColor.g = texture2D(gphTexture0, uv_g).g;
	fragColor.b = texture2D(gphTexture0, uv_b).b;

	// loose luma for some blocks
	if (texture2D(gphTexture1, uv_noise).g < block_thresh)
		fragColor.rgb = fragColor.ggg;

	// discolor block lines
	if (texture2D(gphTexture1, vec2(uv_noise.y, 0.0)).b * 3.5 < line_thresh)
		fragColor.rgb = vec3(0.0, dot(fragColor.rgb, vec3(1.0)), 0.0);

	// interleave lines in some blocks
	if (texture2D(gphTexture1, uv_noise).g * 1.5 < block_thresh ||
		texture2D(gphTexture1, vec2(uv_noise.y, 0.0)).g * 2.5 < line_thresh) {
		float line = fract(fragCoord.y / 3.0);
		vec3 mask = vec3(3.0, 0.0, 0.0);
		if (line > 0.333)
			mask = vec3(0.0, 3.0, 0.0);
		if (line > 0.666)
			mask = vec3(0.0, 0.0, 3.0);
		
		fragColor.xyz *= mask;
	}
}

void main()
{
	vec2 uv = gl_TexCoord[0].xy;
    vec2 coord = uv * texResolution.xy;
    
	vec4 output;
    process(output,coord,texResolution.xy,time);
    
	gl_FragColor = vec4(output.xyz,1.0);
}

#pragma vertex_shader_glx2

void main(){
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
