//
//  PushAnimLayer.h
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/9/2.
//

#ifndef PushAnimLayer_h
#define PushAnimLayer_h

#include "cocos2d.h"
#include "MeteoLayerBase.h"

USING_NS_CC;

class PushAnimLayer : public Layer, public MeteoLayerBase {
public:
    virtual void appearAnimation();
    virtual void disAppearAnimation();
};

#endif /* PushAnimLayer_h */
