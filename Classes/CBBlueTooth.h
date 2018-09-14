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


class CBBlueTooth : public Node
{
private:
    id _blueToothImpl;
    
public:
    CBBlueTooth();
    ~CBBlueTooth();
    
    static CBBlueTooth* getInstance();
    
    bool init();
    bool isPeripheralConnected();
    
    void startScanPeripheral();
    void connectToSelectedPeripheral(ssize_t idx);
    void cancelConnecttionToCurrentPeripheral();
    void writeNotesToPeripheral();
    
    std::pair<std::string, int> getPeripheralByIndex(ssize_t idx);
};

#endif /* CBBlueTooth_h */
