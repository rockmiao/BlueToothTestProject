const char* meteoGaussianBlurShader_frag = R"(
#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform int alphaDecrease;
uniform vec2 pixelSize;
uniform vec2 direction;
uniform int radius;
uniform float weights[64];
uniform int isSelectedColor;

void main()
{
    gl_FragColor = texture2D(CC_Texture0, v_texCoord)*weights[0];
    vec2 offsetStep = pixelSize*direction;
    for (int i = 1; i < radius; i++)
    {
        vec2 offset = float(i)*offsetStep;
        gl_FragColor += texture2D(CC_Texture0, v_texCoord + offset)*weights[i];
        gl_FragColor += texture2D(CC_Texture0, v_texCoord - offset)*weights[i];
    }
    
    gl_FragColor = v_fragmentColor * gl_FragColor;
    
    if (alphaDecrease != 0)
    {
        vec2 referencePoint = v_texCoord;
        float decreaseFactor = 1.;
        
        float d = distance(referencePoint, vec2(.5, .5));
        decreaseFactor = smoothstep(0.70710678118, .4, d);
        
        gl_FragColor.a = gl_FragColor.a * decreaseFactor;
    }
    
    if (isSelectedColor != 0)
        gl_FragColor.rgb = gl_FragColor.rgb*(0.6);
}
)";
