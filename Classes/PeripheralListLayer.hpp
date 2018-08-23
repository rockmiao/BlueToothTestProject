//
//  PeripheralListLayer.hpp
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/8/23.
//

#ifndef PeripheralListLayer_hpp
#define PeripheralListLayer_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class PeripheralListLayer : public Layer, public TableViewDelegate, public TableViewDataSource
{
private:
    Size _visibleSize;
    TableView *_tableView;
public:
    static PeripheralListLayer *create();
    bool init();
    
    //TableView
    virtual void scrollViewDidScroll(ScrollView* view) {};
    virtual void scrollViewDidZoom(ScrollView* view) {};
    virtual void scrollViewDidEndScroll(void) {};
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
};

class PeripheralListCell : public TableViewCell
{
public:
    void refreshCell(ssize_t idx);
};

#endif /* PeripheralListLayer_hpp */
