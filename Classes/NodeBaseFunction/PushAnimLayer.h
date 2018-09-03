//
//  PushAnimLayer.h
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/9/2.
//

#ifndef PushAnimLayer_h
#define PushAnimLayer_h

#include "cocos2d.h"
#include "MeteoChangeLayerBase.h"

USING_NS_CC;

class PushAnimLayer : public Layer, public MeteoChangeLayerBase {
    CC_SYNTHESIZE(ClippingNode *, _visibleClippingNode, VisibleClippingNode);
public:
    virtual bool init();
    
    //動畫 返回動畫時間
    virtual float appearAnimation(bool animation);
    virtual float disappearAnimation(bool animation);
};

#endif /* PushAnimLayer_h */
