//
//  SheetNoteNode.cpp
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/10/1.
//

#include "SheetNoteNode.h"

SheetNoteNode *SheetNoteNode::create(MidiEventList noteOnEvents) {
    SheetNoteNode *ret = new SheetNoteNode();
    if (ret && ret->init(noteOnEvents))
        ret->autorelease();
    else
        CC_SAFE_DELETE(ret);
    return ret;
}

bool SheetNoteNode::init(MidiEventList noteOnEvents) {
    if (!Node::init())
        return false;
    
    unordered_map<int, std::vector<MidiEvent>> durationMap;
    MidiEvent diffEvent;

    for (int i = 0; i < noteOnEvents.size(); i++) {
        diffEvent.setMessage(noteOnEvents[i]);
        diffEvent.recordDuration = noteOnEvents[i].recordDuration;
        durationMap[noteOnEvents[i].getTickDuration()].push_back(diffEvent);
    }
    
    for (auto obj : durationMap) {
        for (int i = 0; i < obj.second.size(); i++) {
            MidiEvent test = obj.second.at(i);
            int noteLength = round(SheetMusicLayer::getTPQ() * 4. / (float)test.getTickDuration());
            unsigned char byte0 = test[0];
            unsigned char byte1 = test[1];
            unsigned char byte2 = test[2];
        }
    }
    
    return true;
}
