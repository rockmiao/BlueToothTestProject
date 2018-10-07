//
//  SheetMusicLayer.h
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/10/1.
//

#ifndef SheetMusicLayer_h
#define SheetMusicLayer_h

#include "cocos2d.h"
#include "../../../ToolBox/miditest2/MidiFile.h"

USING_NS_CC;

using namespace smf;
using namespace std;

typedef std::pair<int, int> SheetSignature;
typedef std::pair<SheetSignature, int> SignatureWithBeginTick;
//typedef std::pair<int, std::pair<int, int>> SignatureRangeInfo;

class SignatureRangeInfo;
class SheetMusicLayer : public Layer {
private:
    bool _needUpdateSignatureInfo;
    
    //test function
    void writeVLValue(long aValue, std::vector<uchar> &outdata);
    void updateSignatureRangeInfo(const std::map<int, SignatureWithBeginTick> &structure, const int &currentTick);
public:
    SheetMusicLayer();
    ~SheetMusicLayer();
    
    static SheetMusicLayer* create(const std::string &fileName);
    bool init(const std::string &fileName);
    
    static int getBarLengthBySignature(SheetSignature signature);
    static int getCurrentBarLengthInTick();
    static int getCurrentBarEndTicks(const int &currentTick, const SignatureWithBeginTick &signatureWithBeginTick);
    
    static SignatureWithBeginTick getCurrentSignature();
    
    void createAcrossNote(MidiEvent &target, MidiEvent &events);
};

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

#endif /* SheetMusicLayer_h */
