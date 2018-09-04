//
//  CodeUtility.hpp
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/9/4.
//

#ifndef CodeUtility_h
#define CodeUtility_h

#include <stdio.h>

typedef std::vector<std::string> StringArray;

class CodeUtility {
public:
    static StringArray splitStringByString(std::string &str, const char *separator);
};

#endif /* CodeUtility_h */
