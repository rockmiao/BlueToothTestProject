//
//  MidiUtility.h
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/9/4.
//

#ifndef MidiUtility_h
#define MidiUtility_h

#include "cocos2d.h"
#include "MeteoMidi.h"

USING_NS_CC;

class MidiUtility {
public:
    static MeteoMidi *readMidiFromFile(const std::string fileName);
};

#endif /* MidiUtility_h */
