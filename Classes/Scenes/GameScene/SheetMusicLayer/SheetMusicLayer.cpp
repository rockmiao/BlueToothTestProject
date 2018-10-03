//
//  SheetMusicLayer.cpp
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/10/1.
//

#include "SheetMusicLayer.h"
#include "../../../ToolBox/SheetMusicUtility.h"
#include "../../../ToolBox/miditest2/MidiFile.h"
#include <iostream>

using namespace smf;
using namespace std;

SheetSignature s_currentSignature;
int TPQ;

SheetMusicLayer::SheetMusicLayer() {
    
}

SheetMusicLayer::~SheetMusicLayer() {
    
}

SheetSignature getCurrentSignature() {
    if (s_currentSignature.first && s_currentSignature.second)
        return s_currentSignature;
    else
        return make_pair(4, 2);
}

SheetMusicLayer* SheetMusicLayer::create(const std::string &fileName) {
    SheetMusicLayer *ret = new SheetMusicLayer();
    if (ret && ret->init(fileName))
        ret->autorelease();
    else
        CC_SAFE_DELETE(ret);
    return ret;
}

bool SheetMusicLayer::init(const std::string &fileName) {
    if ( !Layer::init() )
        return false;
    
    s_currentSignature = make_pair(4, 2);
    
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(fileName);
    MidiFile midifile;
    midifile.read(fullPath);
    midifile.splitTracks();
    midifile.doTimeAnalysis();
    //midifile.splitTracksByChannel();
    midifile.linkNotePairs();
    
    int tracks = midifile.getTrackCount();
    TPQ = midifile.getTicksPerQuarterNote();
    std::vector<MidiEvent> noteOnEvent;
    std::vector<MidiEvent> crossBarEvents;
    
    int currentBarLengthInTicks = getCurrentBarLengthInTick();
    
//    int lastTick = 0;
//    int currTick = 0;
    for (int track = 0; track < tracks; track++) {
        int pauseTime = 0, event = 0, ticks = 0;
        //
        while (event < midifile[track].size()) {
            //先處理每個時間點的MetaData
            while (midifile[track][event].isMeta()) {
                if (midifile[track][event].isTimeSignature()) {
                    s_currentSignature = make_pair((int)midifile[track][event][3], (int)midifile[track][event][4]);
                } else if (midifile[track][event].isTempo()) {
                    //set tempo
                }
                event++;
                ticks = midifile[track][event].tick;
            }
            
            //如果現在的event發生時間點已經超過一個小節了就換下一個小節
            if (midifile[track][event].tick >= currentBarLengthInTicks) {
                
                currentBarLengthInTicks += getCurrentBarLengthInTick();
            }
            
            while (midifile[track][event].tick == ticks) {
                //把發生在同一個ticks的noteOn事件丟去做音符
                if (midifile[track][event].isNoteOn()) {
                    noteOnEvent.push_back(midifile[track][event]);
                    
                    //如果這個noteOn事件持續時間超過一個小節 就把他分成兩個
                    if (midifile[track][event].tick + midifile[track][event].getTickDuration() > currentBarLengthInTicks) {
                        //noteOnEvent.back().isAcrossBar = true;
                        MidiEvent acrossEvent;
                        acrossEvent.tick = 100;
                    }
                }
                
                if (++event < midifile[track].size())
                    break;
            }
        }
    }
    
    
    cout << "TPQ: " << TPQ << endl;
    if (tracks > 1) cout << "TRACKS: " << tracks << endl;
    for (int track=0; track<tracks; track++) {
        if (tracks > 1) cout << "\nTrack " << track << endl;
        cout << "Tick\t\tSeconds\t\tDur\t\tMessage" << endl;
        for (int event=0; event<midifile[track].size(); event++) {
            cout << dec << midifile[track][event].tick;
            cout << '\t' << '\t' << dec << midifile[track][event].seconds;
            cout << '\t' << '\t' ;
            if (midifile[track][event].isNoteOn())
                cout << midifile[track][event].getTickDuration();
            cout << '\t' << '\t' << hex;
            for (int i=0; i<midifile[track][event].size(); i++)
                cout << (int)midifile[track][event][i] << ' ';
            cout << endl;
        }
    }
    
    return true;
}

int SheetMusicLayer::getCurrentBarLengthInTick() {
    return TPQ * s_currentSignature.first * (4 / (1 << s_currentSignature.second));
}
