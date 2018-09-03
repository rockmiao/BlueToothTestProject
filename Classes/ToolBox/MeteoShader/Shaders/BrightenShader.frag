const char* meteoBrightenShader_frag = R"(
#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform float u_time;
uniform int isSelectedColor;

uniform float v;

void main() {
    vec4 color = texture2D(CC_Texture0, v_texCoord);
    vec4 boundary = texture2D(CC_Texture0, v_texCoord);
    float alpha = boundary.a;
    
    float circleTime = sin(u_time / (2. * v));
    
    color = color + vec4(circleTime, circleTime, circleTime, 0.);
    
    if (color[0] < boundary[0])
        color[0] = boundary[0];
    if (color[1] < boundary[1])
        color[1] = boundary[1];
    if (color[2] < boundary[2])
        color[2] = boundary[2];
    
    if (isSelectedColor != 0)
        color.rgb = color.rgb*(0.6);
    
    gl_FragColor = vec4(color.rgb, alpha) * v_fragmentColor;
}
)";
