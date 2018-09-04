//
//  GameMainLayer.cpp
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/9/4.
//

#include "GameMainLayer.h"
#include "../../../ToolBox/MidiUtility.h"

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
    
    MidiUtility::readMidiFromFile("002.mid");
    
    
    return true;
}
