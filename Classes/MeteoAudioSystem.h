//
//  MeteoAudioSystem.h
//  BlueToothTestProject
//
//  Created by miaolin on 2018/9/14.
//

#ifndef MeteoAudioSystem_h
#define MeteoAudioSystem_h

#import <objc/runtime.h>

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;


class MeteoAudioSystem : public Node
{
private:
    id _audioEngineImpl;
    
public:
    MeteoAudioSystem();
    ~MeteoAudioSystem();
    
    static MeteoAudioSystem* getInstance();
    
    bool init();
    
    void notifyStartPlayNote(EventCustom *event);
};

#endif /* MeteoAudioSystem_h */
