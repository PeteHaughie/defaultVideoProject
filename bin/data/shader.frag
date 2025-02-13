OF_GLSL_SHADER_HEADER

#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D tex0;

uniform float xDisplace;
uniform float yDisplace;
uniform float mode;

uniform vec2 resolution;

vec3 rgb2hsb(in vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
    
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

// if we want to work with HSB space in shaders we have to
// convert the RGBA color into an hsb, do some HSB stuff
// and then convert back into RGB for the final draw to the screen
vec3 hsb2rgb(in vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    vec2 uv = gl_FragCoord.xy / resolution.xy;
	vec4 color = texture2D(tex0, vec2(uv.x + xDisplace, uv.y + yDisplace));
    if (mode == 1.0)
        color.rgb = hsb2rgb(color.rgb);
    if (mode == 2.0)
        color.rgb = rgb2hsb(color.rgb);
    gl_FragColor = color;
}
