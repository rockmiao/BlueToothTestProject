//
//  MeteoAudioSystem.m
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/9/14.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#include "MeteoAudioSystem.h"

@interface AVAudioImpl : NSObject
@property (nonatomic, strong) AVAudioEngine *mainEngine;
@property (nonatomic, strong) AVAudioUnitSampler *sampler;
@property (nonatomic, strong) AVAudioUnitReverb *reverb;
@property (nonatomic, strong) AVAudioUnitDelay *delay;
@property (nonatomic, strong) NSMutableArray *samplerArray;

- (void) initAVAudioImpl;
- (void) playNote:(uint8_t)key withVelocity:(uint8_t)withVelocity;
- (void) pauseNote:(uint8_t)key;

@end

#pragma C++ PART

static MeteoAudioSystem* _audioShareInstance = nullptr;

MeteoAudioSystem::MeteoAudioSystem() {
    _audioEngineImpl = [[AVAudioImpl alloc] init];
    
    [(AVAudioImpl *)_audioEngineImpl initAVAudioImpl];
}

MeteoAudioSystem::~MeteoAudioSystem() {
    
}

MeteoAudioSystem* MeteoAudioSystem::getInstance()
{
    if (!_audioShareInstance)
    {
        _audioShareInstance = new (std::nothrow) MeteoAudioSystem();
        CCASSERT(_audioShareInstance, "FATAL: Not enough memory");
        _audioShareInstance->init();
    }
    
    return _audioShareInstance;
}

void MeteoAudioSystem::notifyStartPlayNote(EventCustom *event) {
    if (event->getUserData()) {
        Value *eventValue = (Value*)event->getUserData();
        uint8_t data = eventValue->asByte();
        
        bool hold = !(data%2);
        uint8_t key = data/2;
        
        if (hold)
            [(AVAudioImpl *)_audioEngineImpl playNote:key withVelocity:64];
        else
            [(AVAudioImpl *)_audioEngineImpl pauseNote:key];
    }
}

bool MeteoAudioSystem::init() {
    if (!Node::init())
        return false;
    
    Director::getInstance()->getEventDispatcher()->addCustomEventListener("NotifyStartPlayNote", CC_CALLBACK_1(MeteoAudioSystem::notifyStartPlayNote, this));
    
    return true;
}

#pragma Objective-C PART

@implementation AVAudioImpl

- (void) initAVAudioImpl {
    _mainEngine = [[AVAudioEngine alloc] init];
    _sampler = [[AVAudioUnitSampler alloc] init];
    _reverb = [[AVAudioUnitReverb alloc] init];
    _delay = [[AVAudioUnitDelay alloc] init];
    
    [_mainEngine attachNode:_sampler];
    [_mainEngine attachNode:_reverb];
    [_mainEngine attachNode:_delay];
    
    [_mainEngine connect:_sampler to:_delay format:nil];
    [_mainEngine connect:_delay to:_reverb format:nil];
    [_mainEngine connect:_reverb to:[_mainEngine mainMixerNode] format:nil];
    
    // Reverb
    [_reverb loadFactoryPreset:AVAudioUnitReverbPresetMediumHall];
    [_reverb setWetDryMix:30.0];
    
    // Delay
    [_delay setWetDryMix:15.0];
    [_delay setDelayTime:0.5];
    [_delay setFeedback:75.0];
    [_delay setLowPassCutoff:16000.0];

    if ([_mainEngine isRunning]) {
        return;
    }
    else {
        if (![_mainEngine startAndReturnError:nil]) {
            return;
        }
    }
    AVAudioSession *audioSession = [AVAudioSession sharedInstance];
    
    if (![audioSession setCategory:AVAudioSessionCategoryPlayback withOptions:AVAudioSessionCategoryOptionMixWithOthers error:nil])
        return;
    
    if (![audioSession setActive:true error:nil])
        return;
}

- (void) playNote:(uint8_t)key withVelocity:(uint8_t)withVelocity {
    [_sampler startNote:key withVelocity:withVelocity onChannel:0];
}

- (void) pauseNote:(uint8_t)key {
    [_sampler stopNote:key onChannel:0];
}
@end
