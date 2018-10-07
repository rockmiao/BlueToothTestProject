//
//  SheetNoteNode.h
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/10/1.
//

#ifndef SheetNoteNode_h
#define SheetNoteNode_h

#include "cocos2d.h"
#include "../SheetMusicLayer.h"

USING_NS_CC;

class SheetNoteNode : public Node {
public:
    static SheetNoteNode *create(MidiEventList *noteOnEvents);
    bool init(MidiEventList *noteOnEvents);
};

#endif /* SheetNoteNode_h */
