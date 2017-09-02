
#ifdef VSH

#ifndef BONES_MAX
#define BONES_MAX 32
#endif

uniform mat4 bones_matrix[BONES_MAX];

void armature_transform(vec4 bId, vec4 bWeight, inout vec3 P, inout vec3 N)
{
    vec4 o_v = vec4(P,1.0);
    vec4 o_n = vec4(N,0.0);
    
	vec3 rP = vec3(0.0);
	vec3 rN = vec3(0.0);
	
    //gl_TexCoord[0].xy = gl_MultiTexCoord0.xy;
    //vec4 bId = gl_MultiTexCoord1;
    //vec4 bWeight = gl_MultiTexCoord2;
   
    for(int i = 0; i < 4;i ++){
        float w = bWeight.x;
        int id = int(bId.x);
        if(w <= 0.0001) continue;
        
        rP += (bones_matrix[id] * (o_v * w)).xyz;
        rN += (bones_matrix[id] * (o_n * w)).xyz;
        
        bWeight = bWeight.yzwx;
        bId = bId.yzwx;
    }

    N = normalize(rN);
    P = rP;
}

#endif