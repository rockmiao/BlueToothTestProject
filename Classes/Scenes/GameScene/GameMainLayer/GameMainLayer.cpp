//
//  GameMainLayer.cpp
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/9/4.
//

#include "GameMainLayer.h"
#include "../../../ToolBox/MidiUtility.h"
#include "../../../ToolBox/miditest2/MidiFile.h"
#include <iostream>

using namespace smf;
using namespace std;

GameMainLayer* GameMainLayer::create()
{
    GameMainLayer *ret = new GameMainLayer();
    if (ret && ret->init())
        ret->autorelease();
    else
        CC_SAFE_DELETE(ret);
    return ret;
}

bool GameMainLayer::init()
{
    if ( !PushAnimLayer::init())
        return false;
    
    std::string name = "002.mid";
    name = FileUtils::getInstance()->fullPathForFilename(name);
    MidiFile midifile;
    midifile.read(name);
    midifile.doTimeAnalysis();
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
                cout << midifile[track][event].getDurationInSeconds();
            cout << '\t' << '\t' << hex;
            for (int i=0; i<midifile[track][event].size(); i++)
                cout << (int)midifile[track][event][i] << ' ';
            cout << endl;
        }
    }
    
    
    return true;
}
