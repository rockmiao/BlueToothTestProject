//
//  CBBlueTooth.h
//  BlueToothTestProject
//
//  Created by miaolin on 2018/6/19.
//

#ifndef CBBlueTooth_h
#define CBBlueTooth_h

#import <objc/runtime.h>

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;


class CBBlueTooth : public Layer//, public TableViewDelegate
{
    id _blueToothImpl;
public:
    CBBlueTooth();
    ~CBBlueTooth();
    
    static CBBlueTooth* create();
    bool init();
};

#endif /* CBBlueTooth_h */
