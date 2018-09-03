//
//  LayerInSceneManager.h
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/9/2.
//

#ifndef LayerInSceneManager_h
#define LayerInSceneManager_h

#include "cocos2d.h"
#include "PushAnimLayer.h"
#include "../ToolBox/MeteoDummyTouchLayer.h"

USING_NS_CC;

class ChangeLayerInterface {
public:
    //virtual void addBackKeyEnableLayer(BackKeyLayer *layer, bool enable) = 0;
    
    //virtual void pushToLayerByFlag(PushLayer flag, bool forcePush = false, bool animation = true) = 0;
    virtual void popLayer(int popCount = 1, bool animation = true) = 0;
    virtual void pushToLayer(PushAnimLayer* nextLayer, bool animation = true) = 0;
};

class LayerInSceneManager : public Scene, public ChangeLayerInterface {
    CC_SYNTHESIZE(PushAnimLayer *, _firstLayerInScene, FirstLayerInScene);
    CC_SYNTHESIZE(Size, _visibleSize, VisibleSize);
    CC_SYNTHESIZE(Vec2, _visibleOrigin, VisibleOrigin);
    
private:
    bool _canChangeLayer;
    MeteoDummyTouchLayer *_touchDummy;
    std::vector<PushAnimLayer *> _pushLayerArray;
    
    ClippingNode *createVisibleClipping();
    
    void removePopLayer(int popCount);          //當pop的時後 最後做完動畫要拿掉
    void hidePushLayer();                           //當Push完的時後 前一個做完動畫要hide掉
    
public:
    bool init(PushAnimLayer *firstLayerInScene);
    
    //ChangeLayerInterface
    virtual void popLayer(int popCount = 1, bool animation = true);
    virtual void pushToLayer(PushAnimLayer* nextLayer, bool animation = true);
};

#endif /* LayerInSceneManager_h */
