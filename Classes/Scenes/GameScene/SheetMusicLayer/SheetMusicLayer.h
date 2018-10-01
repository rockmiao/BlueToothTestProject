//
//  SheetMusicLayer.h
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/10/1.
//

#ifndef SheetMusicLayer_h
#define SheetMusicLayer_h

#include "cocos2d.h"

USING_NS_CC;

class SheetMusicLayer : public Layer {
public:
    static SheetMusicLayer* create(const std::string &fileName);
    bool init(const std::string &fileName);
};

#endif /* SheetMusicLayer_h */
