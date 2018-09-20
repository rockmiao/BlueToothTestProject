//
//  LobbySongListLayer.cpp
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/9/19.
//

#include "LobbySongListLayer.h"
#include "../../../NodeBaseFunction/LayerInSceneManager.h"

std::string _songListArray[20] = {"01.adfwdw", "02.wvdwvw wr wrg tgt", "03. tg4 hbt4 4t ", "04.er ger gre g", "05.er g3r g3 3rg rgrg ", "06. egwr g2t2tehge", "07.wr g2rg wertg ", "08.wr egrg wre g", "09.wr wr", "10.wrg 3r gw", "11.wr gwrg wr", "12.wr gwrg ", "13.wr gt hg t4", "14. umuju", "15.y njikiu", "16. uyhunt", "17. uthn uynyn  yny", "18. y ry nhry n", "19.ryh ryh45h 45", "20.2t h25yj hu"};

LobbySongListLayer::LobbySongListLayer() {
    
}

LobbySongListLayer::~LobbySongListLayer() {
    
}

LobbySongListLayer *LobbySongListLayer::create() {
    LobbySongListLayer *ret = new LobbySongListLayer();
    if (ret && ret->init())
        return ret;
    else
        CC_SAFE_DELETE(ret);
    return ret;
}

bool LobbySongListLayer::init() {
    if ( !PushAnimLayer::init() )
        return false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Size viewSize = Size(visibleSize.width/3, visibleSize.height/2);
    
    Label *titleLabel = Label::createWithTTF("Song List", "fonts/Marker Felt.ttf", 18);
    titleLabel->setPosition(Vec2(visibleSize.width/2, visibleSize.height - 50));
    titleLabel->setColor(Color3B::BLACK);
    this->addChild(titleLabel);
    
    Menu *menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    
    Sprite *backButton[2];
    for (int i = 0; i < 2; i++) {
        backButton[i] = Sprite::create("BT_back.png");
        backButton[i]->setColor(i == 0 ? Color3B(120, 120, 120) : Color3B(190, 190, 190));
    }
    
    MenuItemSprite *backItem = MenuItemSprite::create(backButton[0], backButton[1], [=](Ref *sender){
        ChangeLayerInterface *scene = dynamic_cast<ChangeLayerInterface *>(Director::getInstance()->getRunningScene());
        if (scene)
            scene->popLayer();
    });
    backItem->setPosition(Vec2(backItem->getContentSize().width/2 + 5, backItem->getContentSize().height/2 + 5));
    menu->addChild(backItem);
    
    Layer *songListContainer = Layer::create();
    
    _tableView = TableView::create(this, viewSize, songListContainer);
    _tableView->setDirection(TableView::Direction::VERTICAL);
    _tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    _tableView->setDelegate(this);
    _tableView->setAutoAlignment(true);
    _tableView->setPosition(visibleSize.width/2 - viewSize.width/2, visibleSize.height/2 - viewSize.height/2 - 30);
    this->addChild(_tableView);
    
    return true;
}

#pragma ScrollView

void LobbySongListLayer::scrollViewDidScroll(ScrollView* view) {
    
}
void LobbySongListLayer::scrollViewDidZoom(ScrollView* view) {
    
}
void LobbySongListLayer::scrollViewDidEndScroll(void) {
    
}

#pragma TableView

void LobbySongListLayer::tableCellTouched(TableView* table, TableViewCell* cell) {
    
}

void LobbySongListLayer::tableCellHighlight(TableView* table, TableViewCell* cell) {
    
}

void LobbySongListLayer::tableCellUnhighlight(TableView* table, TableViewCell* cell) {
    
}

Size LobbySongListLayer::tableCellSizeForIndex(TableView *table, ssize_t idx) {
    return Size(100, 35);
}

TableViewCell* LobbySongListLayer::tableCellAtIndex(TableView *table, ssize_t idx) {
    LobbySongListCell  *cell = dynamic_cast<LobbySongListCell *>(table->dequeueCell());
    if (!cell) {
        cell = new LobbySongListCell();
        cell->autorelease();
        cell->setAnchorPoint(Vec2(0,0));
    }
    cell->refreshCell(idx);
    
    return cell;
}

ssize_t LobbySongListLayer::numberOfCellsInTableView(TableView *table) {
    return 20;
}

#pragma LobbySongListCell

LobbySongListCell::LobbySongListCell() {
    _songName = Label::createWithTTF("", "fonts/Marker Felt.ttf", 20);
    _songName->setColor(Color3B::BLACK);
    _songName->setAnchorPoint(Vec2(0, 0.5));
    _songName->setPosition(Vec2(0, 35/2));
    this->addChild(_songName);
}

LobbySongListCell::~LobbySongListCell() {
    
}

void LobbySongListCell::refreshCell(ssize_t idx) {
    _songName->setString(_songListArray[idx]);
}
