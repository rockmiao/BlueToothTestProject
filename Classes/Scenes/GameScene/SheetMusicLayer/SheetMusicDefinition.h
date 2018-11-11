//
//  SheetMusicDefinition.h
//  BlueToothTestProject
//
//  Created by Ching-Miao Lin on 2018/11/11.
//

#ifndef SheetMusicDefinition_h
#define SheetMusicDefinition_h

typedef std::pair<int, int> SheetSignature;
typedef std::pair<SheetSignature, int> SignatureWithBeginTick;

class SignatureRangeInfo {
public:
    SignatureRangeInfo():currentBarNo(0),beginBarNo(0),endBarNo(0){}
    
    int currentBarNo;
    int beginBarNo;
    int endBarNo;
    
    void reset() {
        currentBarNo = 0;
        beginBarNo = 0;
        endBarNo = 0;
    };
};

#endif /* SheetMusicDefinition_h */
