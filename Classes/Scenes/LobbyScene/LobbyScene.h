//
//  LobbyScene.h
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/9/2.
//

#ifndef LobbyScene_h
#define LobbyScene_h

#include "cocos2d.h"
#include "../../NodeBaseFunction/LayerInSceneManager.h"

USING_NS_CC;

class LobbyScene : public LayerInSceneManager
{
public:
    static Scene* createScene();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
    CREATE_FUNC(LobbyScene);
};

#endif /* LobbyScene_h */
