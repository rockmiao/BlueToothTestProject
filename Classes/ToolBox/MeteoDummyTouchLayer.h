//
//  MeteoDummyTouchLayer.h
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/9/2.
//

#ifndef MeteoDummyTouchLayer_h
#define MeteoDummyTouchLayer_h

#include "cocos2d.h"

USING_NS_CC;

class MeteoDummyTouchLayer : public Layer{
private:
    Size _touchSize;
public:
    bool init(const Size& touchSize, bool debug = false);
    
    static  MeteoDummyTouchLayer* create(const Size& touchSize, bool debug = false);
    
    virtual bool onTouchBegan(Touch *touch, Event *event);
};

#endif /* MeteoDummyTouchLayer_h */
