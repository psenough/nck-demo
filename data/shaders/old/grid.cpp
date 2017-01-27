
/*float dist = length(position.xy);

float o = dist / 1000.0;
float r = samplePos(vec2(0.0));

r += samplePos(vec2(-o,0.0));
r += samplePos(vec2(o,0.0));
r += samplePos(vec2(0.0,o));
r += samplePos(vec2(0.0,-o));
r += samplePos(vec2(-o,-o));
r += samplePos(vec2(o,o));
r += samplePos(vec2(-o,o));
r += samplePos(vec2(o,-o));

//vec4 grid = texture2D(gphTexture0,gl_TexCoord[0].xy*100.0);
vec4 tex = vec4(0,0,0,1.0);//texture2D(gphTexture1,gl_TexCoord[0].xy*20.0-vec2(0,time*5)/5.0);

//col = (col.r + col.g + col.b) / 3.0;
//col *= vec4(1.0,0.2,0.5,1.0)*0.7;
vec4 pinkish = vec4(1.0,0.2,0.5,1.0);//vec4(1.0,0.5,0.1,1.0);//

float ff = clamp(r/10.0,0,1);
vec4 col = ( ff*(tex+0.2)) * pinkish;

col= mix(pinkish,clamp(col,vec4(0),vec4(1)),(1.0-pow(clamp(o,0,1),1.0)*5.0));
//if(r1 > 0.1)
//col = col * 0.2 + 0.8 * vec4(1.0,0.2,0.5,1.0);

gl_FragColor = col;
*/
