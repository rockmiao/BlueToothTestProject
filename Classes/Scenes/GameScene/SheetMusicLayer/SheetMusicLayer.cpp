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
    int tpq = midifile.getTicksPerQuarterNote();
    int lastTick = 0;
    int currTick = 0;
    for (int track = 0; track < tracks; track++) {
        int pauseTime = 0;
        for (int event=0; event<midifile[track].size(); event++) {
            currTick = midifile[track][event].tick;
            double duration = midifile[track][event].seconds;

            if (midifile[track][event].isNoteOn()) {
                //做休止符
                if (pauseTime != 0) {
                    // ToDo

                    // 歸零
                    pauseTime = 0;
                }
            }
            else {
            }
            lastTick = currTick;
        }
    }
    
    
    cout << "TPQ: " << tpq << endl;
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
