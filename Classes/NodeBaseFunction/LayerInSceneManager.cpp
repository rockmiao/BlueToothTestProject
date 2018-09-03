//
//  LayerInSceneManager.cpp
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/9/2.
//

#include "LayerInSceneManager.h"

bool LayerInSceneManager::init(PushAnimLayer *firstLayerInScene) {
    if (!Scene::init())
        return false;
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    _visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    _firstLayerInScene = firstLayerInScene;
    _pushLayerArray.push_back(firstLayerInScene);
    
    ClippingNode *clipNode = this->createVisibleClipping();
    clipNode->addChild(_firstLayerInScene);
    this->addChild(clipNode);
    _firstLayerInScene->setVisibleClippingNode(clipNode);
    
    _touchDummy = MeteoDummyTouchLayer::create(_visibleSize);
    _touchDummy->setPosition(_visibleOrigin);
    _touchDummy->setVisible(false);
    this->addChild(_touchDummy, INT_MAX);
    
    _canChangeLayer = true;
    
    return true;
}

void LayerInSceneManager::popLayer(int popCount, bool animation) {
    
}

void LayerInSceneManager::removePopLayer()
{
    
}

void LayerInSceneManager::pushToLayer(PushAnimLayer* nextLayer, bool animation) {
    _canChangeLayer = false;
    
    if (nextLayer)
    {
        PushAnimLayer *currentLayer = _pushLayerArray.back();
        
        ClippingNode *nextClippingNode = this->createVisibleClipping();
        nextClippingNode->addChild(nextLayer);
        nextClippingNode->setVisible(false);
        nextLayer->setVisibleClippingNode(nextClippingNode);
        this->addChild(nextClippingNode);
        
        _pushLayerArray.push_back(nextLayer);
        
        if (currentLayer && nextLayer)
        {
            //目前只針對要互相代替的兩個layer做; 之後再考慮要不要對他們所有的child做
            currentLayer->layerWillDisappear();
            nextLayer->layerWillAppear();
            
            currentLayer->stopAllActions();
            nextLayer->stopAllActions();
            
            float disappearTime = currentLayer->disappearAnimation(animation);
            
            this->runAction(Sequence::create(DelayTime::create(disappearTime) , CallFunc::create([=](){
                nextClippingNode->setVisible(true);
                float appearTime = nextLayer->appearAnimation(animation);
                
                nextLayer->runAction(Sequence::create(DelayTime::create(appearTime), CallFunc::create([this](){
                    this->hidePushLayer();
                }), NULL));
            }), NULL));
            
            _touchDummy->setVisible(true);
        }
    }
}

void LayerInSceneManager::hidePushLayer()
{
    _canChangeLayer = true;
    
    if (_pushLayerArray.size() >= 2)
    {
        PushAnimLayer *nextLayer = _pushLayerArray.back();
        PushAnimLayer *currentLayer = _pushLayerArray.at(_pushLayerArray.size() - 2);
        
        if (currentLayer && nextLayer)
        {
            currentLayer->layerDidDisappear();
            nextLayer->layerDidAppear();
            
            currentLayer->getVisibleClippingNode()->setVisible(false);
            
            _touchDummy->setVisible(false);
        }
    }
}

ClippingNode *LayerInSceneManager::createVisibleClipping() {
    ClippingNode *clipNode = ClippingNode::create();
    clipNode->setContentSize(_visibleSize);
    clipNode->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    clipNode->setPosition(_visibleOrigin);
    
    Sprite *stencil = Sprite::create();
    stencil->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    stencil->setTextureRect(Rect(0, 0, _visibleSize.width, _visibleSize.height));
    
    clipNode->setStencil(stencil);
    
    return clipNode;
}
