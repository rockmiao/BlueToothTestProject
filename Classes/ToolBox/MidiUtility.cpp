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
    
    for (int i = 0; i < sizeD; i++)
        CCLOG("%hhd", test[i]);
    
    
    
    return nullptr;
}
