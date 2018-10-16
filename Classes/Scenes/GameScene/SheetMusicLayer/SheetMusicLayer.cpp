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

#define noteHeight 17

SignatureWithBeginTick s_currentSignatureWithBeginTick;
int TPQ;
int s_currentBarEndTicks;
SignatureRangeInfo s_currentSignatureRangeInfo;  //包括目前所在小節, 當下拍號開始小節, 下個拍號開始小節

SheetMusicLayer::SheetMusicLayer():_needUpdateSignatureInfo(true) {
    
}

SheetMusicLayer::~SheetMusicLayer() {
    
}

int SheetMusicLayer::getTPQ() {
    return TPQ;
}

SignatureWithBeginTick getCurrentSignature() {
    if (s_currentSignatureWithBeginTick.first.first && s_currentSignatureWithBeginTick.first.second)
        return s_currentSignatureWithBeginTick;
    else
        return make_pair(make_pair(4, 2), 0);
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
    
    s_currentSignatureWithBeginTick = make_pair(make_pair(4, 2), 0);
    s_currentBarEndTicks = 0;
    
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(fileName);
    MidiFile midifile;
    midifile.read(fullPath);
    midifile.splitTracks();
    midifile.doTimeAnalysis();
    //midifile.splitTracksByChannel();
    midifile.linkNotePairs();
    
    int tracks = midifile.getTrackCount();
    TPQ = midifile.getTicksPerQuarterNote();
    MidiEventList noteOnEvent;// = new MidiEventList;
    MidiEventList crossBarEvents;// = new MidiEventList;
    MidiEvent acrossEvent;
    //std::vector<Node*> needExtendLineNode;
    std::map<int, SignatureWithBeginTick> signatureStructure; //整首歌的拍號分佈 <第幾小節, <開始拍號, tick>>
    signatureStructure[0] = make_pair(make_pair(4, 2), 0);
    
    for (int track = 0; track < tracks; track++) {
        int pauseTime = 0, event = 0, ticks = 0;
        s_currentSignatureRangeInfo.reset();
        s_currentBarEndTicks = 0;
        s_currentSignatureWithBeginTick = signatureStructure[s_currentSignatureRangeInfo.beginBarNo];
        
        while (event < midifile[track].size()) {
            //先處理每個時間點的MetaData
            while (midifile[track][event].isMeta()) {
                if (midifile[track][event].isTimeSignature()) {
                    SignatureWithBeginTick tmp = signatureStructure[s_currentSignatureRangeInfo.beginBarNo];
                    s_currentSignatureRangeInfo.beginBarNo = s_currentSignatureRangeInfo.beginBarNo + ((midifile[track][event].tick - tmp.second) / getBarLengthBySignature(tmp.first));
                    s_currentSignatureRangeInfo.currentBarNo = s_currentSignatureRangeInfo.beginBarNo;
                    s_currentSignatureRangeInfo.endBarNo = -1;
                    s_currentSignatureWithBeginTick =  make_pair(make_pair((int)midifile[track][event][3], (int)midifile[track][event][4]), midifile[track][event].tick);
                    signatureStructure[s_currentSignatureRangeInfo.beginBarNo] = s_currentSignatureWithBeginTick;
                    s_currentBarEndTicks = getCurrentBarEndTicks(midifile[track][event].tick, s_currentSignatureWithBeginTick);
                } else if (midifile[track][event].isTempo()) {
                    //set tempo in BPM
                }
                if (event + 1 >= midifile[track].size())
                    break;
                
                ticks = midifile[track][++event].tick;
            }
            
            //此判斷是專門為第二軌以上 沒有帶拍號資訊的音軌判斷目前拍號用
            if (_needUpdateSignatureInfo &&
                s_currentSignatureRangeInfo.endBarNo != -1 &&
                s_currentSignatureRangeInfo.currentBarNo >= s_currentSignatureRangeInfo.endBarNo) {
                updateSignatureRangeInfo(signatureStructure, midifile[track][event].tick);
                s_currentSignatureWithBeginTick = signatureStructure[s_currentSignatureRangeInfo.beginBarNo];
                s_currentBarEndTicks = getCurrentBarEndTicks(midifile[track][event].tick, s_currentSignatureWithBeginTick);
            }
            
            //如果現在的event發生時間點已經超過一個小節了就換下一個小節
            if (midifile[track][event].tick >= s_currentBarEndTicks) {
                s_currentBarEndTicks = getCurrentBarEndTicks(midifile[track][event].tick, s_currentSignatureWithBeginTick);
                s_currentSignatureRangeInfo.currentBarNo++;
                // todo: 上一小節的延長音補到這小節
            }
            
            noteOnEvent.clear();
//            if (crossBarEvents.size()) {
//                noteOnEvent.assign(crossBarEvents.begin(), crossBarEvents.end());
//                crossBarEvents.clear();
//            }
            int pauseTimeInNote = 0;
            int pauseTimeTemp = pauseTime;
            while (event < midifile[track].size() && midifile[track][event].tick == ticks) {
                //把發生在同一個ticks的noteOn事件丟去做音符
                if (midifile[track][event].isNoteOn()) {
                    float timeGap = (float)TPQ * 4. / (float)(midifile[track][event].tick - pauseTimeTemp);
                    //間距<六連音 就不算間距
                    if (timeGap > 24)
                        timeGap = 0;
                    pauseTimeInNote = round(timeGap);
                    pauseTime = midifile[track][event].tick + midifile[track][event].getTickDuration();
                    noteOnEvent.push_back(midifile[track][event]);
                    noteOnEvent.back().recordDuration = midifile[track][event].getTickDuration();
                    
                    //如果這個noteOn事件持續時間超過一個小節 就把他分成兩個
                    if (midifile[track][event].tick + midifile[track][event].getTickDuration() > s_currentBarEndTicks) {
                        noteOnEvent.back().isAcrossBar = true;
                        //會被回收
                        this->createAcrossNote(midifile[track][event], acrossEvent);
                        crossBarEvents.push_back(acrossEvent);
                        
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
                
                SheetNoteNode *test = SheetNoteNode::create(noteOnEvent);
            }
            //create pauseNote
            if (pauseTimeInNote > 0) {
                
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
    return TPQ * signature.first * (4. / (1 << signature.second));
}

int SheetMusicLayer::getCurrentBarLengthInTick() {
    return getBarLengthBySignature(s_currentSignatureWithBeginTick.first);
}

int SheetMusicLayer::getCurrentBarEndTicks(const int &currentTick, const SignatureWithBeginTick &signatureWithBeginTick) {
    int diffFromLastSignature = currentTick - signatureWithBeginTick.second;
    int currentBarLangth = getBarLengthBySignature(signatureWithBeginTick.first);
    
    return signatureWithBeginTick.second + ((diffFromLastSignature / currentBarLangth + 1) * currentBarLangth);
}

void SheetMusicLayer::createAcrossNote(MidiEvent &target, MidiEvent &events) {
    events.tick = s_currentBarEndTicks;
    events.track = target.track;
    events.isAcrossBar = true;
    events.recordDuration = target.getTickDuration() - (s_currentBarEndTicks - target.tick);
    events.setMessage(target);
//    for (int i = 0; i < target.size(); i++)
//        events.push_back(target.at(i));
}

void SheetMusicLayer::updateSignatureRangeInfo(const std::map<int, SignatureWithBeginTick> &structure, const int &currentTick) {
    int beginBarNo = 0, endBarNo = 0;
    int beginTickTemp = 0;
    SheetSignature beginSignatureTemp;
    for (auto i : structure) {
        if (i.second.second <= currentTick) {
            beginBarNo = i.first;
            beginTickTemp = i.second.second;
            beginSignatureTemp = i.second.first;
        } else {
            endBarNo = i.first;
            break;
        }
    }
    
    if (!endBarNo)
        endBarNo = -1;
    
    int currentBarNo = beginBarNo + ((currentTick - beginTickTemp) / getBarLengthBySignature(beginSignatureTemp));
    
    s_currentSignatureRangeInfo.beginBarNo = beginBarNo;
    s_currentSignatureRangeInfo.endBarNo = endBarNo;
    s_currentSignatureRangeInfo.currentBarNo = currentBarNo;
    
    _needUpdateSignatureInfo = (bool)endBarNo;
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
