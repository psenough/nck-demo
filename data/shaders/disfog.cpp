#pragma vertex_shader_glx2

#include "core.cpp"

varying float dist;

void main()
{
    vec4 P = gl_Vertex;
    
    vec4 wP = gphModelMatrix * P;
    
    dist = length(wP);
    
	gl_Position = gl_ModelViewProjectionMatrix * P;
}

#pragma fragment_shader_glx2

varying float dist;

void main()
{
    vec3 col1 = vec3(0.5,0.5,0.5);
    vec3 col2 = vec3(1.0,1.0,1.0);
	gl_FragColor = vec4(mix(col1, col2, clamp(dist / 62.0,0.0,1.0)), 1.0);
}


