//
//  SheetNoteNode.cpp
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/10/1.
//

#include "SheetNoteNode.h"

SheetNoteNode *SheetNoteNode::create(MidiEventList *noteOnEvents) {
    SheetNoteNode *ret = new SheetNoteNode();
    if (ret && ret->init(noteOnEvents))
        ret->autorelease();
    else
        CC_SAFE_DELETE(ret);
    return ret;
}

bool SheetNoteNode::init(MidiEventList *noteOnEvents) {
    if (!Node::init())
        return false;
    
    unordered_map<int, std::vector<MidiEvent>> durationMap;
    
//    for (int i = 0; i < noteOnEvents[0].size(); i++) {
//        unordered_map<int, std::vector<MidiEvent>>::const_iterator _find = durationMap.find(noteOnEvents.at(i).getTickDuration());
//        if (_find == durationMap.end()) {
//            std::vector<MidiEvent> tmp;
//            tmp.push_back(noteOnEvents.at(i));
//            durationMap[noteOnEvents.at(i).getTickDuration()] = tmp;
//        } else {
//            durationMap[noteOnEvents.at(i).getTickDuration()].push_back(noteOnEvents.at(i));
//        }
//    }
//    
//    for (auto i : durationMap) {
//        auto test1 = i.first;
//        auto test2 = i.second;
//        int t = 0;
//    }
//    
    
    return true;
}
