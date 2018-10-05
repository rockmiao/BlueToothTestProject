//
//  GameMainLayer.cpp
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/9/4.
//

#include "GameMainLayer.h"
#include "../SheetMusicLayer/SheetMusicLayer.h"

GameMainLayer* GameMainLayer::create()
{
    GameMainLayer *ret = new GameMainLayer();
    if (ret && ret->init())
        ret->autorelease();
    else
        CC_SAFE_DELETE(ret);
    return ret;
}

bool GameMainLayer::init()
{
    if ( !PushAnimLayer::init())
        return false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();

    Sprite *sheet = Sprite::create("elements/element-llll-10.png");
    sheet->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    sheet->setPosition(Vec2(0, visibleSize.height - 30));
    this->addChild(sheet);

    Sprite *note = Sprite::create("elements/element-09.png");
    note->setPosition(Vec2(30, sheet->getContentSize().height/2 - note->getContentSize().height));
    sheet->addChild(note);
    
    std::string songName = "test2.mid";
    SheetMusicLayer *sm = SheetMusicLayer::create(songName);
    sm->setPosition(Vec2::ZERO);
    this->addChild(sm);
    
    return true;
}
