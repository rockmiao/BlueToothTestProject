const char* meteoContrastShader_frag = R"(
#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform float threshold;
uniform float increase;
uniform float usingRGBRatio;
uniform int isSelectedColor;

void main() {
    vec4 color = texture2D(CC_Texture0, v_texCoord);
    
    float rRatio = 0.33333333;
    float gRatio = 0.33333333;
    float bRatio = 0.33333333;
    
    if (usingRGBRatio == 1.)
    {
        rRatio = color.r/(color.r+color.g+color.b);
        gRatio = color.g/(color.r+color.g+color.b);
        bRatio = color.b/(color.r+color.g+color.b);
    }
    
    //計算明度 公式有待加強
    float y = 0.299*color.r + 0.587*color.g + 0.114*color.b;
    
    float luminIncrease = 0.;
    if (y >= threshold)
    {
        luminIncrease = smoothstep(threshold, 1., y);
    }
    else
    {
        luminIncrease = smoothstep(0., threshold, y) - 1.;
    }
    
    color.rgb = color.rgb + vec3(rRatio*luminIncrease * increase, gRatio*luminIncrease * increase, bRatio*luminIncrease * increase);
    
    if (isSelectedColor != 0)
        color.rgb = color.rgb*(0.6);
    
    gl_FragColor = color;
}
)";
