//
//  SheetMusicLayer.h
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/10/1.
//

#ifndef SheetMusicLayer_h
#define SheetMusicLayer_h

#include "cocos2d.h"
#include "SheetMusicDefinition.h"
#include "../../../ToolBox/miditest2/MidiFile.h"

USING_NS_CC;

using namespace smf;
using namespace std;

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
    
    static int getTPQ();
    static int getBarLengthBySignature(SheetSignature signature);
    static int getCurrentBarLengthInTick();
    static int getCurrentBarEndTicks(const int &currentTick, const SignatureWithBeginTick &signatureWithBeginTick);
    
    static SignatureWithBeginTick getCurrentSignature();
    
    void createAcrossNote(MidiEvent &target, MidiEvent &events);
};

#endif /* SheetMusicLayer_h */
