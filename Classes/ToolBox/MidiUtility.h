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

typedef unsigned char   uchar;  //1 Byte
typedef unsigned short  ushort; //2 Byte
typedef unsigned long   ulong;  //4 Byte

class MidiUtility {
public:
    static MeteoMidi *readMidiFromFile(const std::string fileName);
    static ulong readLittleEndianBytes(std::string &str, ssize_t &idx, int bytesNum);
    //static ulong readLittleEndain4Bytes(std::string &str, ssize_t &idx);
};

#endif /* MidiUtility_h */
