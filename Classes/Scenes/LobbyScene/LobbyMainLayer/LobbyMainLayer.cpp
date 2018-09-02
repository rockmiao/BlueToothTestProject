//
//  LobbyMainLayer.cpp
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/9/2.
//

#include "LobbyMainLayer.h"

LobbyMainLayer* LobbyMainLayer::create() {
    LobbyMainLayer *ret = new LobbyMainLayer();
    if (ret && ret->init())
        ret->autorelease();
    else
        CC_SAFE_DELETE(ret);
    return ret;
}

bool LobbyMainLayer::init() {
    if (!Layer::init())
        return false;
    
    Label *connectToMachine = Label::createWithTTF("Contect to Meteo", "fonts/Marker Felt.ttf", 18);
    connectToMachine->setColor(Color3B::BLACK);
    connectToMachine->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    connectToMachine->setPosition(Vec2(5, 5));
    
    MenuItemLabel *connectButton = MenuItemLabel::create(connectToMachine, [](Ref *sendor){
        
    });
    
    return true;
}
