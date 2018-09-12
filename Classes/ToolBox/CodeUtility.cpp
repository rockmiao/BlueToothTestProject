//
//  CodeUtility.cpp
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/9/4.
//

#include <sys/stat.h>
#include "dirent.h"
#include "unistd.h"

#include "CodeUtility.h"

static std::string s_ScaleNotationName[12] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

std::string CodeUtility::getScaleNotationName(ssize_t noteNum) {
    return s_ScaleNotationName[(ssize_t)clampf(noteNum, 0, 11)];
}

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


void CodeUtility::renameFiles(std::string filePath, std::string keyWord, int bias) {
    std::vector<std::string> path_vec;
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    
    if((dp=opendir(filePath.c_str()))==NULL) {
        fprintf(stderr,"cannot open %s",filePath.c_str());
        exit(1);
    }
    chdir(filePath.c_str());
    
    while((entry=readdir(dp))!=NULL) {
        stat(entry->d_name,&statbuf);
        if(!S_ISREG(statbuf.st_mode))
            continue;
        path_vec.push_back(StringUtils::format("%s",entry->d_name));
    }
    for (int i = 0; i < path_vec.size(); i++) {
        std::string newName = path_vec.at(i);
        ssize_t pos = newName.find(keyWord);
        if (pos != std::string::npos) {
            std::string currentNoteName = StringUtils::format("%s%d", getScaleNotationName((i + bias)%12).c_str(), (i + bias)/12);
            newName.replace(pos + keyWord.length(), newName.length() - pos - keyWord.length() - 4, currentNoteName);
            rename(path_vec.at(i).c_str(), newName.c_str());
        }
    }
}
