
#pragma vertex_shader_glx2

uniform mat4 gphModelMatrix;
uniform vec4 gphCameraPos;

varying vec3 v_cubemap_normal;
varying vec3 v_cubemap_eye;

void prepare_cubemap(vec4 P, vec3 N){
    v_cubemap_normal = normalize(vec3(gphModelMatrix * vec4(N,0.0))).xyz;
    vec3 pos = (gphModelMatrix * P).xyz;
    v_cubemap_eye = -normalize(pos - gphCameraPos.xyz);
}

void main()
{
    vec4 P = gl_Vertex;
	vec3 N = gl_Normal;
    prepare_cubemap(P,N);
    gl_Position = gl_ModelViewProjectionMatrix * P;
}

#pragma fragment_shader_glx2

varying vec3 v_cubemap_normal;
varying vec3 v_cubemap_eye;

uniform samplerCube gphTexture0;

vec4 cubemap_reflect(samplerCube tex){
    vec3 n = normalize(v_cubemap_normal);
    vec3 e = normalize(v_cubemap_eye);
    vec3 v = reflect(e, n);
    return textureCube(tex, v);
}

vec4 cubemap_refract(samplerCube tex, float factor){
    vec3 n = normalize(v_cubemap_normal);
    vec3 e = normalize(v_cubemap_eye);
    vec3 v = refract(e, n, factor);
    return textureCube(tex, v);
}

void main()
{
    gl_FragColor = cubemap_reflect(gphTexture0);
}


