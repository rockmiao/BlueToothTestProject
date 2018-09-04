//
//  CodeUtility.cpp
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/9/4.
//

#include "CodeUtility.h"

StringArray CodeUtility::splitStringByString(std::string &str, const char *separator)
{
    StringArray result;
    std::string temp;
    std::string totalString(str);
    size_t pos = totalString.find(separator, 0);
    
    while (pos != std::string::npos)
    {
        temp = totalString.substr(0, pos);
        result.push_back(temp);
        
        totalString.erase(0, pos + strlen(separator));
        pos = totalString.find(separator, 0);
    }
    
    result.push_back(totalString);
    
    return result;
}
