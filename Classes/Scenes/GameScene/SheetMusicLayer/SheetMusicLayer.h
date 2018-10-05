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

class SheetMusicLayer : public Layer {
private:
    
    //test function
    void writeVLValue(long aValue, std::vector<uchar>& outdata);
public:
    SheetMusicLayer();
    ~SheetMusicLayer();
    
    static SheetMusicLayer* create(const std::string &fileName);
    bool init(const std::string &fileName);
    
    static int getBarLengthBySignature(SheetSignature signature);
    static int getCurrentBarLengthInTick();
    static int getCurrentBarEndTicks(const int &currentTick, const SignatureWithBeginTick &signatureWithBeginTick);
    
    static SheetSignature getCurrentSignature();
    
    MidiEvent createAcrossNote(MidiEvent &target);
    void analyzeMetaTrack(std::unordered_map<int, SheetSignature> &signatureStructure, MidiEventList list);
};

#endif /* SheetMusicLayer_h */
