//
//  MeteoDummyTouchLayer.cpp
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/9/2.
//

#include "MeteoShaders.h"
#include "MeteoShaderSprite.h"
#include "Shaders/LightningShader.frag"
#include "Shaders/BrightenShader.frag"
#include "Shaders/ContrastShader.frag"
#include "Shaders/GaussianBlurShader.frag"

static ShaderMap _shaderMap;

#define STRINGIFY(A)  #A

const char* meteoWhiteColorSpriteShader_frag = STRINGIFY(
    \n#ifdef GL_ES\n
    precision lowp float;
    \n#endif\n

    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;

    void main()
    {
      gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
      gl_FragColor = vec4(1.0 * gl_FragColor.a, 1.0 * gl_FragColor.a, 1.0 * gl_FragColor.a, gl_FragColor.a);
    }
);

const char* meteoGrayColorSpriteShader_frag = STRINGIFY(
    \n#ifdef GL_ES\n
    precision lowp float;
    \n#endif\n

    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;

    void main()
    {
        vec4 c = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
        float gray = 0.2126*c.r + 0.7152*c.g + 0.0722*c.b;
        gray = gray * c.a;
        gl_FragColor = vec4(gray, gray, gray, c.a);
    }
);

#pragma mark - 這邊開始MeteoShaders

void MeteoShaders::initSprite(MeteoShaderSprite *sprite, bool isSelectedColor)
{
    _programState = _programState->clone();
    _programState->setUniformInt("isSelectedColor", isSelectedColor);
    
    setUniform();
}

GLProgramState* MeteoShaders::getShaderState(ShaderType shaderType, bool isMVP)
{
    if (!_shaderMap.size())
        MeteoShaders::loadShaderMap();
    
    std::pair<const char*, std::pair<std::string, std::string>> tmp;
    tmp = _shaderMap.at(shaderType);
    
    std::string shaderKey = isMVP ? tmp.second.first : tmp.second.second;
    GLProgramState *glState = GLProgramState::getOrCreateWithGLProgramName(shaderKey);
    if (!glState)
    {
        GLProgram* glProgram = GLProgram::createWithByteArrays(isMVP ? ccPositionTextureColor_vert : ccPositionTextureColor_noMVP_vert, tmp.first);
        
        GLProgramCache::getInstance()->addGLProgram(glProgram, shaderKey);
        
        glState = GLProgramState::getOrCreateWithGLProgramName(shaderKey);
    }
    
    return glState;
}

void MeteoShaders::loadShaderMap()
{
    _shaderMap.clear();
    std::string mvpStringKey[SHADER_NUMBER + 1] = {
        "ShaderPositionTextureColor",
        "Meteo_WHITE_COLOR_MVP_SPRITE_SHADER",
        "Meteo_GRAY_MVP_SPRITE_SHADER",
        "Meteo_BRIGHTEN_MVP_SPRITE_SHADER",
        "Meteo_LIGHTNING_MVP_SPRITE_SHADER",
        "Meteo_CONTRACT_MVP_SPRITE_SHADER",
        "Meteo_GAUSSIAN_BLUR_MVP_SPRITE_SHADER"
    };
    std::string noMvpStringKey[SHADER_NUMBER + 1] = {
        "ShaderPositionTextureColor_noMVP",
        "Meteo_WHITE_COLOR_SPRITE_SHADER",
        "Meteo_GRAY_SPRITE_SHADER",
        "Meteo_BRIGHTEN_SPRITE_SHADER",
        "Meteo_LIGHTNING_SPRITE_SHADER",
        "Meteo_CONTRACT_SPRITE_SHADER",
        "Meteo_GAUSSIAN_BLUR_SPRITE_SHADER"
    };
    
    const char* fragShaders[SHADER_NUMBER + 1] = {
        ccPositionTextureColor_frag,
        meteoWhiteColorSpriteShader_frag,
        meteoGrayColorSpriteShader_frag,
        meteoBrightenShader_frag,
        meteoLightningShader_frag,
        meteoContrastShader_frag,
        meteoGaussianBlurShader_frag
    };
    
    for (int i = 0; i < SHADER_NUMBER + 1; i++)
    {
        const char *programChar = fragShaders[i];
        
        std::pair<const char*, std::pair<std::string, std::string>> tmp;
        tmp = make_pair(programChar, make_pair(mvpStringKey[i], noMvpStringKey[i]));
        
        _shaderMap.insert(ShaderMap::value_type((ShaderType)i, tmp));
    }
}

GLProgramState* MeteoShaders::getWhiteShaderState(bool isMVP)
{
    return MeteoShaders::getShaderState(ShaderType::WhiteShaderState, isMVP);
}

GLProgramState* MeteoShaders::getGrayShaderState(bool isMVP)
{
    return MeteoShaders::getShaderState(ShaderType::GrayShaderState, isMVP);
}

#pragma mark - 這邊開始是BrightenShader

MeteoBrightenShader* MeteoBrightenShader::create(float v)
{
    MeteoBrightenShader *ret = new MeteoBrightenShader();
    ret->init(v);
    ret->autorelease();
    
    return ret;
}

bool MeteoBrightenShader::init(float v)
{
    _v = v;
    _needUpdateTime = true;
    return true;
}

void MeteoBrightenShader::initSprite(MeteoShaderSprite *sprite, bool isSelectedColor)
{
    _programState = MeteoShaders::getShaderState(ShaderType::BrightenShaderState);
    
    sprite->setBlendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED);
    
    MeteoShaders::initSprite(sprite, isSelectedColor);
}

void MeteoBrightenShader::setUniform()
{
    _programState->setUniformFloat("v", _v);
}

void MeteoBrightenShader::updateUniformTime(float delta)
{
    if (_programState)
    {
        _updateTime += delta;
        if (_updateTime >= _v * 2)
            _updateTime = _updateTime - _v * 2;
        float time = _updateTime * 2 * M_PI;
        _programState->setUniformFloat("u_time", time);
    }
}

#pragma mark - 這邊開始是LightningShader

MeteoLightningShader* MeteoLightningShader::create(float a,float b, float v, float lightWidth, float secLightWidth, float gradient, float brightness)
{
    MeteoLightningShader* ret = new MeteoLightningShader();
    ret->init(a, b, v, lightWidth, secLightWidth, gradient, brightness);
    ret->autorelease();
    
    return ret;
}

bool MeteoLightningShader::init(float a,float b, float v, float lightWidth, float secLightWidth, float gradient, float brightness)
{
    _needUpdateTime = true;
    _a = a;
    _b = b;
    _v = v;
    _lightWidth = lightWidth;
    _secLightWidth = secLightWidth;
    _gradient = gradient;
    _brightness = brightness;
    _textureSize = Vec2(1024, 1024);
    
    return true;
}

void MeteoLightningShader::initSprite(MeteoShaderSprite *sprite, bool isSelectedColor)
{
    _programState = MeteoShaders::getShaderState(ShaderType::LightningShaderState);
    
    _textureSize = sprite->getTexture()->getContentSizeInPixels();
    
    MeteoShaders::initSprite(sprite, isSelectedColor);
}

void MeteoLightningShader::setUniform()
{
    _programState->setUniformFloat("u_time", 0);
    _programState->setUniformFloat("a", _a);
    _programState->setUniformFloat("b", _b);
    _programState->setUniformFloat("lightWidth", _lightWidth);
    _programState->setUniformFloat("secLightWidth", _secLightWidth);
    _programState->setUniformFloat("gradient", _gradient);
    _programState->setUniformFloat("brightness", _brightness);
    _programState->setUniformVec2("textureSize", _textureSize);
}

void MeteoLightningShader::updateUniformTime(float delta)
{
    if (_programState)
    {
        _updateTime += delta;
        float time = (_updateTime*_v - floor(_updateTime*_v));
        _programState->setUniformFloat("u_time", time);
    }
}

#pragma mark - 這邊開始是MeteoContrastShader

MeteoContrastShader* MeteoContrastShader::create(float threshold, float increase, bool usingRGBRatio)
{
    MeteoContrastShader* ret = new MeteoContrastShader();
    ret->init(threshold, increase, usingRGBRatio);
    ret->autorelease();
    
    return ret;
}

bool MeteoContrastShader::init(float threshold, float increase, bool usingRGBRatio)
{
    _needUpdateTime = false;
    _threshold = threshold;
    _increase = increase;
    _usingRGBRatio = usingRGBRatio;
    
    return true;
}

void MeteoContrastShader::initSprite(MeteoShaderSprite *sprite, bool isSelectedColor)
{
    _programState = MeteoShaders::getShaderState(ShaderType::ContractShaderState);
    
    MeteoShaders::initSprite(sprite, isSelectedColor);
}

void MeteoContrastShader::setUniform()
{
    _programState->setUniformFloat("threshold", _threshold);
    _programState->setUniformFloat("increase", _increase);
    _programState->setUniformFloat("usingRGBRatio", (int)_usingRGBRatio);
}

#pragma mark - 這邊開始是MeteoGaussianBlurShader

MeteoGaussianBlurShader* MeteoGaussianBlurShader::create(Vec2 direction, int radius, int step, bool alphaDecrease)
{
    MeteoGaussianBlurShader* ret = new MeteoGaussianBlurShader();
    ret->init(direction, radius, step, alphaDecrease);
    ret->autorelease();
    
    return ret;
}

bool MeteoGaussianBlurShader::init(Vec2 direction, int radius, int step, bool alphaDecrease)
{
    _needUpdateTime = false;
    _radius = radius;
    _step = step;
    _direction = direction;
    _alphaDecrease = alphaDecrease;
    
    return true;
}

void MeteoGaussianBlurShader::initSprite(MeteoShaderSprite *sprite, bool isSelectedColor)
{
    _programState = MeteoShaders::getShaderState(ShaderType::GaussianBlurShaderState);
    
    getGaussianWeight(_radius/_step, _weights);
    Size textureSize = sprite->getSpriteFrame()->getRectInPixels().size;
    _pixelSize = Vec2(float(_step)/textureSize.width, float(_step)/textureSize.height);

    MeteoShaders::initSprite(sprite, isSelectedColor);
}

void MeteoGaussianBlurShader::setUniform()
{
    _programState->setUniformInt("alphaDecrease", _alphaDecrease);
    _programState->setUniformVec2("pixelSize", _pixelSize);
    _programState->setUniformVec2("direction", _direction);
    _programState->setUniformInt("radius", _radius);
    _programState->setUniformFloatv("weights", _radius, _weights);
}

void MeteoGaussianBlurShader::getGaussianWeight(const int points, float* weights)
{
    float dx = 1.0f/float(points-1);
    float sigma = 1.0f/3.0f;
    float norm = 1.0f/(sqrtf(2.0f*M_PI)*sigma*points);
    float divsigma2 = 0.5f/(sigma*sigma);
    weights[0] = 1.0f;
    for (int i = 1; i < points; i++)
    {
        float x = float(i)*dx;
        weights[i] = norm*expf(-x*x*divsigma2);
        weights[0] -= 2.0f*weights[i];
    }
}
