//
//  GameMainLayer.h
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/9/4.
//

#ifndef GameMainLayer_h
#define GameMainLayer_h

#include "cocos2d.h"
#include "../../../NodeBaseFunction/PushAnimLayer.h"

USING_NS_CC;

class GameMainLayer : public PushAnimLayer {
public:
    static GameMainLayer* create();
    virtual bool init();
    
    //MeteoChangeLayerBase
    virtual void layerWillAppear() {};
    virtual void layerDidAppear() {};
    virtual void layerWillDisappear() {};
    virtual void layerDidDisappear() {};
};

#endif /* GameMainLayer_h */
