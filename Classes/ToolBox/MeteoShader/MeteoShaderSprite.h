//
//  MeteoDummyTouchLayer.h
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/9/2.
//

#ifndef __Texas_MeteoShaderSprite_H__
#define __Texas_MeteoShaderSprite_H__

#include "cocos2d.h"
#include "MeteoShaders.h"

USING_NS_CC;

class MeteoShaderSprite : public Sprite{
    CC_SYNTHESIZE(bool, _drawToRenderTexture, DrawToRenderTexture)
private:
    MeteoShaders *_shader;
public:
    ~MeteoShaderSprite();
    MeteoShaderSprite():_shader(nullptr), _drawToRenderTexture(false){};
    
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    
    static MeteoShaderSprite* create(const std::string& filename);
    
    static MeteoShaderSprite* createWithSpriteFrameName(const std::string& spriteFrameName);
    
    static MeteoShaderSprite* createWithSpriteFrame(SpriteFrame *spriteFrame);
    
    static MeteoShaderSprite* createWithTexture(Texture2D *texture);
    
    //抓Shader Program
    static GLProgramState* getWhiteShaderState(bool isMVP = false);     //變白
    static GLProgramState* getGrayShaderState(bool isMVP = false);      //變灰
    
    //是否使用變白的Shader
    void setUseWhiteShader(bool var);
    
    //是否使用變灰的Shader
    void setUseGrayShader(bool var);
    
    void setShader(MeteoShaders* shader, bool isSelectedColor = false);
    MeteoShaders* getShader() { return _shader; };
    
    void resetShader();
    
    void freezeShaderMotion(bool freeze);
    
    void update(float delta);
};

#pragma 一些shader可能會用到的工具

class MeteoShaderSpriteTools : public Node
{
public:
    //將傳入的Node轉換成一張Sprite
    static MeteoShaderSprite* changeNodeToMeteoShaderSprite(Node *target);
    
    //將傳入的ShaderedSprite的SpriteFrame改成渲染過後的
    static MeteoShaderSprite* changeToStaticFilteredSprite(MeteoShaderSprite *target, bool getClone = false);
    
private:
    static Vec4 culculateChildBoundInParentsView(Node *target);
    static Vec4 culBounds(Node *target);
};

#endif

