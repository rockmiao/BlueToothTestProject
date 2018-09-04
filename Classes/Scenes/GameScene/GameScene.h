//
//  GameScene.h
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/9/4.
//

#ifndef GameScene_h
#define GameScene_h

#include "cocos2d.h"
#include "../../NodeBaseFunction/LayerInSceneManager.h"

USING_NS_CC;

class GameScene : public LayerInSceneManager {
public:
    static Scene* createScene();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
    CREATE_FUNC(GameScene);
};

#endif /* GameScene_h */
