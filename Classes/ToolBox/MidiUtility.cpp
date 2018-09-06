//
//  MidiUtility.cpp
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/9/4.
//

#include "MidiUtility.h"
#include "CodeUtility.h"

MeteoMidi *MidiUtility::readMidiFromFile(const std::string fileName) {
    
    std::string test = FileUtils::getInstance()->getStringFromFile(fileName);
    
    ssize_t sizeD = test.length();
    char *stringData = new char [sizeD + 1];
    
    std::strcpy(stringData, test.c_str());
    
    StringArray array = CodeUtility::splitStringByString(test, "MTrk");
    
    // Read header file
    ssize_t ptr = 4;
    ushort shortData;
    ulong  longData;
    
    longData = readLittleEndianBytes(array.at(0), ptr, 4);
    CCASSERT(longData == 6, "This file is not a MIDI 1.0 Standard MIDI file.");
    
    int type;
    shortData = readLittleEndianBytes(array.at(0), ptr, 2);
    switch (shortData) {
        case 0:
            type = 0;
            break;
        case 1:
            type = 1;
            break;
        case 2:
        default:
            CCASSERT(false, "Error: cannot handle this type of Midi file");
    }
    
    int tracks;
    shortData = readLittleEndianBytes(array.at(0), ptr, 2);
    CCASSERT(shortData == array.size() - 1, "This file is not a MIDI 1.0 Standard MIDI file.");
    tracks = shortData;
    
    shortData = readLittleEndianBytes(array.at(0), ptr, 2);
    if (shortData >= 0x8000) {
        int framespersecond = 255 - ((shortData >> 8) & 0x00ff) + 1;
        int subframes       = shortData & 0x00ff;
        switch (framespersecond) {
            case 25:  framespersecond = 25; break;
            case 24:  framespersecond = 24; break;
            case 29:  framespersecond = 29; break;  // really 29.97 for color television
            case 30:  framespersecond = 30; break;
            //"Warning: unknown / non-standard FPS"
            default:
                break;
        }
        //m_ticksPerQuarterNote = framespersecond * subframes;
        
        // std::cerr << "SMPTE ticks: " << m_ticksPerQuarterNote << " ticks/sec" << std::endl;
        // std::cerr << "SMPTE frames per second: " << framespersecond << std::endl;
        // std::cerr << "SMPTE subframes per frame: " << subframes << std::endl;
    }  else {
        //m_ticksPerQuarterNote = shortData;
    }
    
    for (int i = 1; i < array.size(); i++)
    {
        
    }
    
    return nullptr;
}

ulong MidiUtility::readLittleEndianBytes(std::string &str, ssize_t &idx, int bytesNum) {
    CCASSERT(bytesNum <= 4, "'bytesNum' mustn't greater than 4");
    uchar bytes[bytesNum];
    ulong res = 0;
    
    for (int i = 0; i < bytesNum; i++)
    {
        CCASSERT(idx < str.length(), "Error: unexpected end of file.");
        bytes[i] = str[idx++];
        
        res = res | (bytes[i] << (bytesNum - i - 1) * 8);
    }
    
    return res;
}
