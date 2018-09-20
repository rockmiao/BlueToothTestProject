//
//  LobbySongListLayer.h
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/9/19.
//

#ifndef LobbySongListLayer_h
#define LobbySongListLayer_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../../../NodeBaseFunction/PushAnimLayer.h"
#include "../../../ToolBox/MeteoCyclingTableView.h"

USING_NS_CC;
USING_NS_CC_EXT;

class LobbySongListLayer : public PushAnimLayer, public TableViewDelegate, public TableViewDataSource {
private:
    TableView *_tableView;
    
public:
    LobbySongListLayer();
    ~LobbySongListLayer();
    
    static LobbySongListLayer *create();
    bool init();
    
    //TableView
    virtual void scrollViewDidScroll(ScrollView* view);
    virtual void scrollViewDidZoom(ScrollView* view);
    virtual void scrollViewDidEndScroll(void);
    
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

class LobbySongListCell : public TableViewCell {
private:
    Label *_songName;
public:
    LobbySongListCell();
    ~LobbySongListCell();
    
    void refreshCell(ssize_t idx);
};

#endif /* LobbySongListLayer_h */
