//
//  MeteoShaders.h
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/9/2.
//
#ifndef MeteoShaders_h
#define MeteoShaders_h

#include "cocos2d.h"

#define SHADER_NUMBER 6

USING_NS_CC;

enum ShaderType{
    None                    = 0,
    WhiteShaderState        = 1,        //白色
    GrayShaderState         = 2,        //灰色
    BrightenShaderState     = 3,        //漸亮
    LightningShaderState    = 4,        //流光
    ContractShaderState     = 5,        //對比
    GaussianBlurShaderState = 6         //高斯模糊
};

typedef std::map<ShaderType, std::pair<const char*, std::pair<std::string, std::string>>> ShaderMap;

class MeteoShaderSprite;
class MeteoShaders : public Ref
{
    CC_SYNTHESIZE_READONLY(bool, _needUpdateTime, NeedUpdateTime);
protected:
    float _updateTime;
    
    GLProgramState* _programState;
    
public:
    //抓Shader Program
    static GLProgramState* getWhiteShaderState(bool isMVP = false);     //變白
    static GLProgramState* getGrayShaderState(bool isMVP = false);      //變灰
    
    static GLProgramState* getShaderState(ShaderType shaderType, bool isMVP = false);
    static void loadShaderMap();
    
public:
    MeteoShaders() {
        _programState = nullptr;
        _updateTime = 0;
    };
    
    virtual void initSprite(MeteoShaderSprite *sprite, bool isSelectedColor);
    virtual void updateUniformTime(float delta){};
    virtual void setUniform() = 0;
    
    GLProgramState* getGLProgramState() { return _programState; };
};

class MeteoBrightenShader : public MeteoShaders
{
protected:
    float _v;       //正常->最亮->正常 總共多久
    
public:
    bool init(float v);
    
    static MeteoBrightenShader* create(float v = 1);
    
    virtual void initSprite(MeteoShaderSprite *sprite, bool isSelectedColor);
    virtual void updateUniformTime(float delta);
    virtual void setUniform();
};

class MeteoLightningShader : public MeteoShaders
{
protected:
    float _a;               //直線方程式的x係數
    float _b;               //直線方程式的y係數
    float _v;               //光要滑多快(一秒的幾倍走完一整張texture
    float _lightWidth;      //光多寬
    float _secLightWidth;   //第二條光要多寬
    float _gradient;        //光的邊緣模糊程度
    float _brightness;      //光要多亮
    Vec2 _textureSize;      //使用的textureSize多大 校正用
    
public:
    bool init(float a,float b, float v, float lightWidth, float secLightWidth, float gradient, float brightness);
    
    static MeteoLightningShader* create(float a = 1,float b = 1, float v = 0.15, float lightWidth = 0.1, float secLightWidth = 0.02, float gradient = 0.004, float brightness = 3);
    
    virtual void initSprite(MeteoShaderSprite *sprite, bool isSelectedColor);
    virtual void updateUniformTime(float delta);
    virtual void setUniform();
};

class MeteoContrastShader : public MeteoShaders
{
protected:
    float _threshold;   //亮度大於多少會更亮 小於多少更暗
    float _increase;    //差距程度
    bool _usingRGBRatio;//是否依照原本rgb的比例增加亮度
    
public:
    bool init(float threshold, float increase, bool usingRGBRatio);
    static MeteoContrastShader* create(float threshold = 0.35, float increase = 2, bool usingRGBRatio = true);
    
    virtual void initSprite(MeteoShaderSprite *sprite, bool isSelectedColor);
    virtual void setUniform();
};

/*
 * 要讓圖片模糊的話 需要搭配 MeteoShaderSpriteTools::changeToStaticFilteredSprite()
 * 分別做一次垂直模糊 + 一次水平模糊
 */

class MeteoGaussianBlurShader : public MeteoShaders
{
protected:
    bool _alphaDecrease;    //是否邊緣淡化
    int _radius;            //每個取樣點要跟半徑多少以內的其他點混合
    int _step;              //每個取樣點與待混合的其他點的距離一次跳多少
    float _weights[64];     //高斯參數
    Vec2 _pixelSize;        //根據_step決定一次移動的距離
    Vec2 _direction;        //模糊的方向
    
private:
    void getGaussianWeight(const int points, float* weights);   //算出高斯參數（a.k.a.此點與鄰近的點的混合比例
public:
    bool init(Vec2 direction, int radius, int step, bool alphaDecrease);
    static MeteoGaussianBlurShader* create(Vec2 direction = Vec2(1, 0), int radius = 15, int step = 1, bool alphaDecrease = false);
    
    virtual void initSprite(MeteoShaderSprite *sprite, bool isSelectedColor);
    virtual void setUniform();
};

#endif /* MeteoShaders_h */

