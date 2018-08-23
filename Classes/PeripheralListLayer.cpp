//
//  PeripheralListLayer.cpp
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/8/23.
//

#include "PeripheralListLayer.hpp"
#include "CBBlueTooth.h"

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
    
    CBBlueTooth *BLEView = CBBlueTooth::create();
    this->addChild(BLEView);
    
    //TableView
    Layer *mainLayer = Layer::create();
    _tableView = TableView::create(this, Size(_visibleSize.width, _visibleSize.height - 64), mainLayer);
    _tableView->setDirection(TableView::Direction::VERTICAL);
    _tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    _tableView->setVisible(false);
    _tableView->setDelegate(this);
    this->addChild(_tableView);
    
    Director::getInstance()->getEventDispatcher()->addCustomEventListener("NotifyRefreshDataList", [=](EventCustom *event){
        
    });
    
    return true;
}

#pragma TableViewDataSource

void PeripheralListLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}

void PeripheralListLayer::tableCellHighlight(TableView* table, TableViewCell* cell)
{
    
}

void PeripheralListLayer::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{
    
}

Size PeripheralListLayer::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(_visibleSize.width, 30);
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
    cell->refreshCell(idx);
    
    return cell;
}

ssize_t PeripheralListLayer::numberOfCellsInTableView(TableView *table)
{
    return 0;
}

#pragma TableViewCell

void PeripheralListCell::refreshCell(ssize_t idx)
{
    
}
