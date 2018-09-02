//
//  LayerInSceneManager.cpp
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/9/2.
//

#include "LayerInSceneManager.h"

bool LayerInSceneManager::init(MeteoLayerBase *firstLayerInScene) {
    if (!Scene::init())
        return false;
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    _visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    _firstLayerInScene = firstLayerInScene;
    
    ClippingNode *clipNode = ClippingNode::create();
    clipNode->setContentSize(_visibleSize);
    clipNode->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    clipNode->setPosition(_visibleOrigin);
    this->addChild(clipNode);
    
    Sprite *stencil = Sprite::create();
    stencil->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    stencil->setTextureRect(Rect(0, 0, _visibleSize.width, _visibleSize.height));
    
    clipNode->setStencil(stencil);
    
    Layer *child = dynamic_cast<Layer *>(_firstLayerInScene);
    if (child)
        clipNode->addChild(child);
    
    return true;
}
