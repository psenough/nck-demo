
#pragma vertex_shader_glx2

varying vec3 screenPos;

void main()
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
    vec4 projected = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = projected;
    screenPos = projected.xyz;
	gl_FrontColor = gl_Color;
}

#pragma fragment_shader_glx2

varying vec3 screenPos;
uniform sampler2D gphTexture0;

void main()
{
    //vec4 tex = texture2D (gphTexture0,0.5*screenPos.xy*vec2(1.0,9.0/16.9));
    vec2 fc = screenPos.xy*0.5;
    vec2 res = vec2(16.0/9.0,1.0);
    
       vec2 uv = fc.xy / res.xy;
       
    if (fract(-10.0 * uv.x + 10.0* uv.y) > 0.5)
        gl_FragColor = vec4(1.0);
    else
        gl_FragColor = vec4(0.0);
    
    //gl_FragColor = vec4(tex);
}


