//
//  LobbyScene.cpp
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/9/2.
//

#include "LobbyScene.h"
#include "LobbyMainLayer/LobbyMainLayer.h"
#include "../../CBBlueTooth.h"
#include "../../MeteoAudioSystem.h"

Scene* LobbyScene::createScene() {
    return LobbyScene::create();
}

bool LobbyScene::init() {
    if (!LayerInSceneManager::init(LobbyMainLayer::create()))
        return false;
    
    LayerColor *sceneBG = LayerColor::create(Color4B::WHITE, _visibleSize.width, _visibleSize.height);
    sceneBG->setPosition(_visibleOrigin);
    this->addChild(sceneBG, INT_MIN);
    
    CBBlueTooth::getInstance();
    MeteoAudioSystem::getInstance();
    
    return true;
}

void LobbyScene::onEnter() {
    Scene::onEnter();
    
}

void LobbyScene::onEnterTransitionDidFinish() {
    Scene::onEnterTransitionDidFinish();
    
}

void LobbyScene::onExit() {
    // todo
    Scene::onExit();
}
