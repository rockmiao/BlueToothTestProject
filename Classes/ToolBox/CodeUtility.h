//
//  CodeUtility.hpp
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/9/4.
//

#ifndef CodeUtility_h
#define CodeUtility_h

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

typedef std::vector<std::string> StringArray;

class CodeUtility {
public:
    static StringArray splitStringByString(std::string &str, const char *separator);
    static void renameFiles(std::string filePath, std::string keyWord, int bias = 0);
    static std::string getScaleNotationName(ssize_t noteNum);
};

#endif /* CodeUtility_h */
