//
//  LobbyMainLayer.cpp
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/9/2.
//

#include "LobbyMainLayer.h"
#include "../PeripheralListLayer/PeripheralListLayer.h"
#include "../LobbySongListLayer/LobbySongListLayer.h"
#include "../../GameScene/GameScene.h"
#include "../../../ToolBox/CodeUtility.h"
#include "../../../NodeBaseFunction/LayerInSceneManager.h"

LobbyMainLayer* LobbyMainLayer::create() {
    LobbyMainLayer *ret = new LobbyMainLayer();
    if (ret && ret->init())
        ret->autorelease();
    else
        CC_SAFE_DELETE(ret);
    return ret;
}

bool LobbyMainLayer::init() {
    if (!PushAnimLayer::init())
        return false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    Menu *menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    
    //連接meteo
    Label *connectToMachine = Label::createWithTTF("Contect to Meteo", "fonts/Marker Felt.ttf", 18);
    connectToMachine->setColor(Color3B::BLACK);
    
    MenuItemLabel *connectButton = MenuItemLabel::create(connectToMachine, [](Ref *sendor){
        LayerInSceneManager *scene = dynamic_cast<LayerInSceneManager *>(Director::getInstance()->getRunningScene());
        
        if (scene)
            scene->pushToLayer(PeripheralListLayer::create());
    });
    connectButton->setPosition(Vec2(5 + connectButton->getContentSize().width/2 * 1.2, 5 + connectButton->getContentSize().height/2 * 1.2));
    menu->addChild(connectButton);
    
    //歌單
    Label *songList = Label::createWithTTF("Song List", "fonts/Marker Felt.ttf", 24);
    songList->setColor(Color3B(120, 120, 120));
    
    MenuItemLabel *songListButton = MenuItemLabel::create(songList, [](Ref *sendor){
        Director::getInstance()->replaceScene(GameScene::createScene());
    });
    songListButton->setPosition(Vec2(visibleSize.width/2 + 30, visibleSize.height/2 + 60));
    songListButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    menu->addChild(songListButton);
    
    //商店
    Label *store = Label::createWithTTF("Song Store", "fonts/Marker Felt.ttf", 24);
    store->setColor(Color3B(120, 120, 120));
    
    MenuItemLabel *storeButton = MenuItemLabel::create(store, CC_CALLBACK_1(LobbyMainLayer::songStoreClick, this));
    storeButton->setPosition(Vec2(visibleSize.width/2 + 80, visibleSize.height/2));
    storeButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    menu->addChild(storeButton);
    
    //我的歌單
    Label *mySongList = Label::createWithTTF("My Song List", "fonts/Marker Felt.ttf", 24);
    mySongList->setColor(Color3B(120, 120, 120));
    
    MenuItemLabel *mySongListButton = MenuItemLabel::create(mySongList, [](Ref *sendor){
        LayerInSceneManager *scene = dynamic_cast<LayerInSceneManager *>(Director::getInstance()->getRunningScene());
        
        if (scene)
            scene->pushToLayer(LobbySongListLayer::create());
    });
    mySongListButton->setPosition(Vec2(visibleSize.width/2 + 30, visibleSize.height/2 - 60));
    mySongListButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    menu->addChild(mySongListButton);
    
    
    return true;
}

void LobbyMainLayer::songStoreClick(Ref *sender)
{
    CodeUtility::renameFiles("/Users/miaolin/miaolinWorkingSpace/wav_files", "German_Concert_", 9);
}
