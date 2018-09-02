//
//  LobbyMainLayer.h
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/9/2.
//

#ifndef LobbyMainLayer_h
#define LobbyMainLayer_h

#include "cocos2d.h"
#include "../../../NodeBaseFunction/PushAnimLayer.h"

class LobbyMainLayer : public PushAnimLayer {
public:
    static LobbyMainLayer* create();
    virtual bool init();
    
    //MeteoLayerBase
    virtual void layerWillAppear() {};
    virtual void layerDidAppear() {};
    virtual void layerWillDisappear() {};
    virtual void layerDidDisappear() {};
};

#endif /* LobbyMainLayer_h */
