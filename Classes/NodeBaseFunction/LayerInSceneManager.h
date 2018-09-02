//
//  LayerInSceneManager.h
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/9/2.
//

#ifndef LayerInSceneManager_h
#define LayerInSceneManager_h

#include "cocos2d.h"
#include "MeteoLayerBase.h"

USING_NS_CC;

class LayerInSceneManager : public Scene
{
    CC_SYNTHESIZE(MeteoLayerBase *, _firstLayerInScene, FirstLayerInScene);
    CC_SYNTHESIZE(Size, _visibleSize, VisibleSize);
    CC_SYNTHESIZE(Vec2, _visibleOrigin, VisibleOrigin);
public:
    bool init(MeteoLayerBase *firstLayerInScene);
};

#endif /* LayerInSceneManager_h */
