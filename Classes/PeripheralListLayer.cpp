//
//  PeripheralListLayer.cpp
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/8/23.
//

#include "PeripheralListLayer.hpp"

PeripheralListLayer::~PeripheralListLayer()
{
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("NotifyRefreshDataList");
}

PeripheralListLayer *PeripheralListLayer::create()
{
    PeripheralListLayer *ret = new PeripheralListLayer();
    if (ret && ret->init())
        ret->autorelease();
    else
        CC_SAFE_DELETE(ret);
    return ret;
}

bool PeripheralListLayer::init()
{
    if ( !Layer::init() )
        return false;
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    CBBlueTooth::getInstance()->startScanPeripheral();
    
    Size viewSize = Size(_visibleSize.width/2, _visibleSize.height - 64);
    
    //TableView
    Layer *mainLayer = Layer::create();
    _tableView = TableView::create(this, viewSize, mainLayer);
    _tableView->setDirection(TableView::Direction::VERTICAL);
    _tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    _tableView->setDelegate(this);
    _tableView->setPosition(_visibleSize.width/2 + origin.x - viewSize.width/2, _visibleSize.height/2 + origin.y - viewSize.height/2);
    this->addChild(_tableView);
    
    Director::getInstance()->getEventDispatcher()->addCustomEventListener("NotifyRefreshDataList", [=](EventCustom *event){
        cocos2d::Value eventValue = *(cocos2d::Value*) event->getUserData();
        _cellNumber = eventValue.asInt();
        
        _tableView->reloadData();
    });
    
    return true;
}

#pragma TableViewDataSource

void PeripheralListLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    ssize_t idx = ((PeripheralListCell*)cell)->getPeripheralIdx();
    CBBlueTooth::getInstance()->connectToSelectedPeripheral(idx);
}

void PeripheralListLayer::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    
}

void PeripheralListLayer::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    
}

Size PeripheralListLayer::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(_visibleSize.width/2, 30);
}

TableViewCell* PeripheralListLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
    PeripheralListCell *cell = dynamic_cast<PeripheralListCell*>(table->dequeueCell());
    if (!cell)
    {
        cell = new PeripheralListCell();
        cell->autorelease();
        cell->setAnchorPoint(Vec2(0,0));
    }
    //std::pair<std::string, int> data = _BLEView->getPeripheralByIndex(idx);
    std::pair<std::string, int> data = CBBlueTooth::getInstance()->getPeripheralByIndex(idx);
    cell->refreshCell(data, idx);
    
    return cell;
}

ssize_t PeripheralListLayer::numberOfCellsInTableView(TableView *table)
{
    return _cellNumber;
}

#pragma TableViewCell

PeripheralListCell::PeripheralListCell()
{
    _peripheralIdx = 0;
    
    _mark = ui::Scale9Sprite::create("bg_information6.png");
    _mark->setPreferredSize(Size(Director::getInstance()->getVisibleSize().width/2, 30));
    _mark->setAnchorPoint(Vec2(0, 0));
    this->addChild(_mark);
    
    _name = Label::createWithSystemFont("", "HelveticaNeue", 16);
    _name->setColor(Color3B::WHITE);
    _name->setAnchorPoint(Vec2(0, 0.5));
    _name->setPosition(Vec2(20, 15));
    this->addChild(_name);
    
    _rssi = Label::createWithSystemFont("", "HelveticaNeue", 16);
    _rssi->setColor(Color3B::WHITE);
    _rssi->setAnchorPoint(Vec2(1, 0.5));
    _rssi->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2 - 10, 15));
    this->addChild(_rssi);
}

PeripheralListCell::~PeripheralListCell()
{
    
}

void PeripheralListCell::refreshCell(std::pair<std::string, int> nameRSSI, ssize_t idx)
{
    _peripheralIdx = idx;
    
    _name->setString(nameRSSI.first);
    
    _rssi->setString(StringUtils::format("%d", nameRSSI.second));
    
}
