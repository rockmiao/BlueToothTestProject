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
    if (!_canChangeLayer)
        return;
    ssize_t layerCount = _pushLayerArray.size();
    
    if (layerCount <= 1)
        return;
    
    _canChangeLayer = false;
    
    if (popCount == -1 || popCount >= layerCount - 1)
        popCount = layerCount - 1;
    
    PushAnimLayer *currentLayer = _pushLayerArray.back();
    PushAnimLayer *goalLayer = _pushLayerArray.at(layerCount - 2);
    
    if (currentLayer && goalLayer) {
        currentLayer->layerWillDisappear();
        goalLayer->layerWillAppear();
        
        currentLayer->stopAllActions();
        goalLayer->stopAllActions();
        
        float disappearTime = currentLayer->disappearAnimation(animation);
        
        this->runAction(Sequence::create(DelayTime::create(disappearTime) , CallFunc::create([=](){
            currentLayer->getVisibleClippingNode()->setVisible(false);
            goalLayer->getVisibleClippingNode()->setVisible(true);
            float appearTime = goalLayer->appearAnimation(animation);
            
            goalLayer->runAction(Sequence::create(DelayTime::create(appearTime), CallFunc::create([this, popCount](){
                this->removePopLayer(popCount);
            }), NULL));
        }), NULL));
        
        _touchDummy->setVisible(true);
    }
}

void LayerInSceneManager::removePopLayer(int popCount) {
    _canChangeLayer = true;
    ssize_t pushLayerCount = _pushLayerArray.size();
    if (pushLayerCount > 1) {
        PushAnimLayer *disappearNode = _pushLayerArray.back();
        PushAnimLayer *appearNode = _pushLayerArray.at(pushLayerCount - popCount - 1);
        
        if (disappearNode && appearNode) {
            disappearNode->layerDidDisappear();
            
            for(int i = 0 ; i < popCount ; i++) {
                auto lastNode = _pushLayerArray.back();
                lastNode->removeFromParentAndCleanup(true);
                lastNode = nullptr;
                _pushLayerArray.pop_back();
            }
            
            appearNode->layerDidAppear();
            
            _touchDummy->setVisible(false);
        }
    }
}

void LayerInSceneManager::pushToLayer(PushAnimLayer* nextLayer, bool animation) {
    _canChangeLayer = false;
    
    if (nextLayer) {
        PushAnimLayer *currentLayer = _pushLayerArray.back();
        
        ClippingNode *nextClippingNode = this->createVisibleClipping();
        nextClippingNode->addChild(nextLayer);
        nextClippingNode->setVisible(false);
        nextLayer->setVisibleClippingNode(nextClippingNode);
        this->addChild(nextClippingNode);
        
        _pushLayerArray.push_back(nextLayer);
        
        if (currentLayer && nextLayer) {
            //目前只針對要互相代替的兩個layer做; 之後再考慮要不要對他們所有的child做
            currentLayer->layerWillDisappear();
            nextLayer->layerWillAppear();
            
            currentLayer->stopAllActions();
            nextLayer->stopAllActions();
            
            float disappearTime = currentLayer->disappearAnimation(animation);
            
            this->runAction(Sequence::create(DelayTime::create(disappearTime) , CallFunc::create([=](){
                currentLayer->getVisibleClippingNode()->setVisible(false);
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

void LayerInSceneManager::hidePushLayer() {
    _canChangeLayer = true;
    
    if (_pushLayerArray.size() >= 2) {
        PushAnimLayer *nextLayer = _pushLayerArray.back();
        PushAnimLayer *currentLayer = _pushLayerArray.at(_pushLayerArray.size() - 2);
        
        if (currentLayer && nextLayer) {
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
