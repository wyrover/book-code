//
//  PersonalAudioEngine.cpp
//  FishingJoy
//
//  Created by Ringo_D on 12-9-27.
//
//

#include "PersonalAudioEngine.h"
#include "FishingJoyData.h"
#include "StaticData.h"
static PersonalAudioEngine* _sharedEngine = NULL;
using namespace CocosDenshion;
USING_NS_CC;
void PersonalAudioEngine::setBackgroundMusicVolume(float volume)
{
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(volume);
    FishingJoyData::sharedFishingJoyData()->setMusicVolume(volume);
}
void PersonalAudioEngine::setEffectsVolume(float volume)
{
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(volume);
    FishingJoyData::sharedFishingJoyData()->setSoundVolume(volume);
}
PersonalAudioEngine* PersonalAudioEngine::sharedEngine()
{
    if(_sharedEngine==NULL){
        _sharedEngine = new PersonalAudioEngine();
        _sharedEngine->init();
    }
    return _sharedEngine;
}
bool PersonalAudioEngine::init()
{
    this->preloadBackgroundMusic(STATIC_DATA_STRING("bg_music"));
    this->preloadEffect(STATIC_DATA_STRING("sound_button"));
    this->preloadEffect(STATIC_DATA_STRING("sound_shot"));

    this->setBackgroundMusicVolume(FishingJoyData::sharedFishingJoyData()->getMusicVolume());
    this->setEffectsVolume(FishingJoyData::sharedFishingJoyData()->getSoundVolume());
    
    return true;
}
void PersonalAudioEngine::purge()
{

}
PersonalAudioEngine::PersonalAudioEngine()
{
    
}
PersonalAudioEngine::~PersonalAudioEngine()
{
    
}
