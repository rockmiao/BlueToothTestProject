//
//  SheetMusicLayer.cpp
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/10/1.
//

#include "SheetMusicLayer.h"
#include "SheetNoteNode/SheetNoteNode.h"
#include "../../../ToolBox/SheetMusicUtility.h"
#include <iostream>

SheetSignature s_currentSignature;
int TPQ;
int s_currentBarEndTicks;
int s_currentSignatureFromBar;      //目前的signature是從哪一個bar開始的

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
    
    long aValue = 3840;
    std::vector<uchar> outdata;
    
    writeVLValue(aValue, outdata);
    
    s_currentSignature = make_pair(4, 2);
    s_currentBarEndTicks = 0;
    s_currentSignatureFromBar = 0;
    
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
    //std::vector<Node*> needExtendLineNode;
    std::unordered_map<int, SignatureWithBeginTick> signatureStructure; //整首歌的拍號分佈 <第幾小節, <開始拍號, tick>>
    signatureStructure[0] = make_pair(make_pair(4, 2), 0);
    
    for (int track = 0; track < tracks; track++) {
        int pauseTime = 0, event = 0, ticks = 0;
        
        while (event < midifile[track].size()) {
            //先處理每個時間點的MetaData
            while (midifile[track][event].isMeta()) {
                if (midifile[track][event].isTimeSignature()) {
                    SignatureWithBeginTick tmp = signatureStructure[s_currentSignatureFromBar];
                    s_currentSignatureFromBar = s_currentSignatureFromBar + ((midifile[track][event].tick - tmp.second) / getBarLengthBySignature(tmp.first));
                    signatureStructure[s_currentSignatureFromBar] = make_pair(make_pair((int)midifile[track][event][3], (int)midifile[track][event][4]), midifile[track][event].tick);
                    s_currentSignature = signatureStructure[s_currentSignatureFromBar].first;
                } else if (midifile[track][event].isTempo()) {
                    //set tempo
                }
                if (event + 1 >= midifile[track].size())
                    break;
                
                ticks = midifile[track][++event].tick;
            }
            
            //如果現在的event發生時間點已經超過一個小節了就換下一個小節
            if (midifile[track][event].tick >= s_currentBarEndTicks) {
                s_currentBarEndTicks = getCurrentBarEndTicks(midifile[track][event].tick, signatureStructure[s_currentSignatureFromBar]);
                // todo: 上一小節的延長音補到這小節
            }
            
            noteOnEvent.clear();
            if (crossBarEvents.size()) {
                noteOnEvent.assign(crossBarEvents.begin(), crossBarEvents.end());
                crossBarEvents.clear();
            }
            
            while (event < midifile[track].size() && midifile[track][event].tick == ticks) {
                //把發生在同一個ticks的noteOn事件丟去做音符
                if (midifile[track][event].isNoteOn()) {
                    noteOnEvent.push_back(midifile[track][event]);
                    
                    //如果這個noteOn事件持續時間超過一個小節 就把他分成兩個
                    if (midifile[track][event].tick + midifile[track][event].getTickDuration() > s_currentBarEndTicks) {
                        noteOnEvent.back().isAcrossBar = true;
                        //會被回收
//                        MidiEvent acrossEvent = this->createAcrossNote(midifile[track][event]);
//                        crossBarEvents.push_back(acrossEvent);
                        
                        //換下一小節的時候要把所有偵測到有across的note都連到下一小節去
                        //needExtendLineNode.push_back(currentNode);
                    }
                }
                
                event++;
            }
            
            if (event == midifile[track].size())
                ticks = midifile[track][event - 1].tick;
            else
                ticks = midifile[track][event].tick;
            
            //把音畫上去
            if (noteOnEvent.size()) {
                int pauseTimeInTick = noteOnEvent.at(0).tick - pauseTime;
                pauseTime = noteOnEvent.at(0).tick;
                //create pauseNote
                SheetNoteNode *test = SheetNoteNode::create(noteOnEvent);
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

int SheetMusicLayer::getBarLengthBySignature(SheetSignature signature) {
    return TPQ * signature.first * (4 / (1 << signature.second));
}

int SheetMusicLayer::getCurrentBarLengthInTick() {
    return getBarLengthBySignature(s_currentSignature);
}

int SheetMusicLayer::getCurrentBarEndTicks(const int &currentTick, const SignatureWithBeginTick &signatureWithBeginTick) {
    int diffFromLastSignature = currentTick - signatureWithBeginTick.second;
    int currentBarLangth = getBarLengthBySignature(signatureWithBeginTick.first);
    
    return signatureWithBeginTick.second + ((diffFromLastSignature / currentBarLangth + 1) * currentBarLangth);
}

MidiEvent SheetMusicLayer::createAcrossNote(MidiEvent &target) {
    MidiEvent ret;
    ret.tick = s_currentBarEndTicks;
    ret.isAcrossBar = true;
    ret.extendDuration = target.getTickDuration() - (s_currentBarEndTicks - target.tick);
    for (int i = 0; i < target.size(); i++)
        ret.push_back(target.at(i));
    
    return ret;
}

void SheetMusicLayer::analyzeMetaTrack(std::unordered_map<int, SheetSignature> &signatureStructure, MidiEventList list) {
    
}


void SheetMusicLayer::writeVLValue(long aValue, std::vector<uchar>& outdata) {
    uchar bytes[4] = {0};
    
    if ((unsigned long)aValue >= (1 << 28)) {
        std::cerr << "Error: number too large to convert to VLV" << std::endl;
        aValue = 0x0FFFffff;
    }
    
    bytes[0] = (uchar)(((ulong)aValue >> 21) & 0x7f);  // most significant 7 bits
    bytes[1] = (uchar)(((ulong)aValue >> 14) & 0x7f);
    bytes[2] = (uchar)(((ulong)aValue >> 7)  & 0x7f);
    bytes[3] = (uchar)(((ulong)aValue)       & 0x7f);  // least significant 7 bits
    
    int start = 0;
    while ((start<4) && (bytes[start] == 0))  start++;
    
    for (int i=start; i<3; i++) {
        bytes[i] = bytes[i] | 0x80;
        outdata.push_back(bytes[i]);
    }
    outdata.push_back(bytes[3]);
}
