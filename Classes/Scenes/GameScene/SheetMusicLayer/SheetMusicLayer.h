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

class SheetMusicLayer : public Layer {
private:
public:
    SheetMusicLayer();
    ~SheetMusicLayer();
    
    static int getCurrentBarLengthInTick();
    static SheetSignature getCurrentSignature();
    static SheetMusicLayer* create(const std::string &fileName);
    bool init(const std::string &fileName);
    MidiEvent createAcrossNote(MidiEvent &target);
};

#endif /* SheetMusicLayer_h */
