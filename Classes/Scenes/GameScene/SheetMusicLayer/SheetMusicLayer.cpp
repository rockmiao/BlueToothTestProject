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
    
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(fileName);
    MidiFile midifile;
    midifile.read(fullPath);
    midifile.splitTracks();
    midifile.doTimeAnalysis();
    //midifile.splitTracksByChannel();
    midifile.linkNotePairs();
    
    int tracks = midifile.getTrackCount();
    cout << "TPQ: " << midifile.getTicksPerQuarterNote() << endl;
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
