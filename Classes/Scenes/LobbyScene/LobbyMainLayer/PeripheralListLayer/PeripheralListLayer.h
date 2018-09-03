//
//  PeripheralListLayer.h
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/8/23.
//

#ifndef PeripheralListLayer_h
#define PeripheralListLayer_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../../../../CBBlueTooth.h"
#include "../../../../NodeBaseFunction/PushAnimLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

class PeripheralListLayer : public PushAnimLayer, public TableViewDelegate, public TableViewDataSource
{
private:
    Size _visibleSize;
    TableView *_tableView;
    int _cellNumber;
    
//    CBBlueTooth *_BLEView;
public:
    ~PeripheralListLayer();
    
    static PeripheralListLayer *create();
    bool init();
    
    void notifyCharacteristicConnectedSendData(EventCustom *event);
    
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
    
    //MeteoChangeLayerBase
    virtual void layerWillAppear() {};
    virtual void layerDidAppear() {};
    virtual void layerWillDisappear() {};
    virtual void layerDidDisappear() {};
};

class PeripheralListCell : public TableViewCell
{
    CC_SYNTHESIZE(ssize_t, _peripheralIdx, PeripheralIdx)
private:
    ui::Scale9Sprite *_mark;
    Label *_name;
    Label *_rssi;
    
public:
    PeripheralListCell();
    ~PeripheralListCell();
    
    void refreshCell(std::pair<std::string, int> nameRSSI, ssize_t idx);
};

#endif /* PeripheralListLayer_h */
