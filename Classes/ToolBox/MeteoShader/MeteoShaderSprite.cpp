//
//  MeteoDummyTouchLayer.cpp
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/9/2.
//

#include "MeteoShaderSprite.h"
#include "MeteoShaders.h"

MeteoShaderSprite::~MeteoShaderSprite()
{
    CC_SAFE_RELEASE_NULL(_shader);
}

MeteoShaderSprite* MeteoShaderSprite::create(const std::string& filename)
{
    MeteoShaderSprite *sprite = new (std::nothrow) MeteoShaderSprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

MeteoShaderSprite* MeteoShaderSprite::createWithSpriteFrameName(const std::string& spriteFrameName)
{
    SpriteFrame *spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
    
    MeteoShaderSprite *sprite = new (std::nothrow) MeteoShaderSprite();
    if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

MeteoShaderSprite* MeteoShaderSprite::createWithSpriteFrame(SpriteFrame *spriteFrame)
{
    MeteoShaderSprite *sprite = new (std::nothrow) MeteoShaderSprite();
    if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

MeteoShaderSprite* MeteoShaderSprite::createWithTexture(Texture2D *texture)
{
    MeteoShaderSprite *sprite = new (std::nothrow) MeteoShaderSprite();
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void MeteoShaderSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    if (_drawToRenderTexture)
    {
        _trianglesCommand.init(_globalZOrder, _texture, getGLProgramState(), _blendFunc, _polyInfo.triangles, transform, 0);
        renderer->addCommand(&_trianglesCommand);
    }
    else
    {
        Sprite::draw(renderer, transform, flags);
    }
}

void MeteoShaderSprite::setShader(MeteoShaders* shader, bool isSelectedColor)
{
    if (_shader != shader)
    {
        CC_SAFE_RELEASE_NULL(_shader);
        
        _shader = shader;
        _shader->retain();
    }
    
    _shader->initSprite(this, isSelectedColor);
    if (_shader->getGLProgramState())
    {
        setGLProgramState(_shader->getGLProgramState());
    }
    
    if (_shader->getNeedUpdateTime())
        this->scheduleUpdate();
    else
        this->unscheduleUpdate();
}

void MeteoShaderSprite::resetShader()
{
    if (_shader)
    {
        if (_shader->getNeedUpdateTime())
            this->unscheduleUpdate();
        
        CC_SAFE_RELEASE_NULL(_shader);
    }
    
    GLProgramState* state = MeteoShaders::getShaderState(ShaderType::None);
    if (state)
        setGLProgramState(state);
}

void MeteoShaderSprite::update(float delta)
{
    _shader->updateUniformTime(delta);
}

void MeteoShaderSprite::freezeShaderMotion(bool freeze)
{
    if (_shader && _shader->getNeedUpdateTime())
    {
        if (freeze)
            this->unscheduleUpdate();
        else
            this->scheduleUpdate();
    }
}

#pragma mark - 是否使用變白的Shader

void MeteoShaderSprite::setUseWhiteShader(bool var)
{
    if(var)
    {
        //要變白
        setGLProgramState(MeteoShaders::getWhiteShaderState());
    }
    else
    {
        setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
    }
}

#pragma mark - 是否使用變灰的Shader

void MeteoShaderSprite::setUseGrayShader(bool var)
{
    if(var)
    {
        //要變灰
        setGLProgramState(MeteoShaders::getGrayShaderState());
    }
    else
    {
        setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
    }
}

#pragma mark - MeteoShaderSpriteTools

MeteoShaderSprite* MeteoShaderSpriteTools::changeToStaticFilteredSprite(MeteoShaderSprite *target, bool getClone)
{
    BlendFunc pureBlend = { GL_ONE, GL_ZERO };
    
    MeteoShaderSprite *puppet = MeteoShaderSprite::createWithSpriteFrame(target->getSpriteFrame());
    puppet->setDrawToRenderTexture(true);
    puppet->setAnchorPoint(Vec2::ZERO);
    target->getShader()->setUniform();
    if (target->getShader()->getNeedUpdateTime())
        target->update(0);
    
    GLProgramState *temp = target->getGLProgramState()->clone();
    puppet->setGLProgramState(temp);
    puppet->setBlendFunc(pureBlend);
    
    //把渲染後的替身存起來丟給本身
    RenderTexture *tmpImg = RenderTexture::create(target->getContentSize().width, target->getContentSize().height);
    puppet->setFlippedY(true);
    tmpImg->beginWithClear(0, 0, 0, 0);
    puppet->visit();
    tmpImg->end();
    
    if (getClone)
    {
        MeteoShaderSprite *clone = MeteoShaderSprite::createWithSpriteFrame(tmpImg->getSprite()->getSpriteFrame());
        return clone;
    }
    else
    {
        target->setSpriteFrame(tmpImg->getSprite()->getSpriteFrame());
        target->resetShader();
        return nullptr;
    }
}

MeteoShaderSprite* MeteoShaderSpriteTools::changeNodeToMeteoShaderSprite(Node *target)
{
    Vec2 tmpPos = target->getPosition();
    
    target->setPosition(Vec2::ZERO);
    
    Vec4 childrenBound = MeteoShaderSpriteTools::culculateChildBoundInParentsView(target);
    
    target->setPosition(Vec2(-childrenBound.x, -childrenBound.y));
    
    Size textureSize = Size(childrenBound.z - childrenBound.x, childrenBound.w - childrenBound.y);
    
    RenderTexture *tmpImg = RenderTexture::create(textureSize.width, textureSize.height);
    
    tmpImg->begin();
    target->visit();
    tmpImg->end();
    
    MeteoShaderSprite *res = MeteoShaderSprite::createWithSpriteFrame(tmpImg->getSprite()->getSpriteFrame());
    
    Vec2 anchorPoint = Vec2(-childrenBound.x/(childrenBound.z - childrenBound.x),
                            -childrenBound.y/(childrenBound.w - childrenBound.y));
    res->setAnchorPoint(anchorPoint);
    
    return res;
}

Vec4 MeteoShaderSpriteTools::culculateChildBoundInParentsView(Node *target)
{
    // Vec4(leftBound, bottomBound, rightBound, topBound)
    Vec4 bounds = MeteoShaderSpriteTools::culBounds(target);
    Vec2 origin = Vec2(bounds.x, bounds.y);
    
    for (auto *child : target->getChildren())
    {
        Vec4 childBound = MeteoShaderSpriteTools::culculateChildBoundInParentsView(child);
        childBound.x += origin.x;
        childBound.y += origin.y;
        childBound.z += origin.x;
        childBound.w += origin.y;
        
        if (childBound.x < bounds.x)
            bounds.x = childBound.x;
        if (childBound.y < bounds.y)
            bounds.y = childBound.y;
        if (childBound.z > bounds.z)
            bounds.z = childBound.z;
        if (childBound.w > bounds.w)
            bounds.w = childBound.w;
    }
    
    return bounds;
}

Vec4 MeteoShaderSpriteTools::culBounds(Node *target)
{
    Vec2 pos = target->getPosition();
    Vec2 anchor = target->getAnchorPoint();
    Size contentSize = target->getContentSize();
    
    Vec4 res = Vec4(-anchor.x*contentSize.width*target->getScaleX() + pos.x,
                    -anchor.y*contentSize.height*target->getScaleY() + pos.y,
                    (1-anchor.x)*contentSize.width*target->getScaleX() + pos.x,
                    (1-anchor.y)*contentSize.height*target->getScaleY() + pos.y);
    
    return res;
}
