//
//  GameScene.cpp
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/9/4.
//

#include "GameScene.h"
#include "GameMainLayer/GameMainLayer.h"

Scene* GameScene::createScene() {
    return GameScene::create();
}

bool GameScene::init() {
    if (!LayerInSceneManager::init(GameMainLayer::create()))
        return false;
    
    LayerColor *sceneBG = LayerColor::create(Color4B::WHITE, _visibleSize.width, _visibleSize.height);
    sceneBG->setPosition(_visibleOrigin);
    this->addChild(sceneBG, INT_MIN);
    
    return true;
}

void GameScene::onEnter() {
    Scene::onEnter();
    
}

void GameScene::onEnterTransitionDidFinish() {
    Scene::onEnterTransitionDidFinish();
    
}

void GameScene::onExit() {
    // todo
    Scene::onExit();
}
