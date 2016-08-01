//
//  FishingJoyData.cpp
//  FishingJoy
//
//  Created by Ringo_D on 12-9-30.
//
//

#include "FishingJoyData.h"
#include "StaticData.h"
USING_NS_CC;
static FishingJoyData* _sharedFishingJoyData = NULL;
FishingJoyData* FishingJoyData::sharedFishingJoyData()
{
    if(_sharedFishingJoyData==NULL){
        _sharedFishingJoyData = new FishingJoyData();
        _sharedFishingJoyData->init();
    }
    return _sharedFishingJoyData;
}
void FishingJoyData::purge()
{
    CC_SAFE_RELEASE_NULL(_sharedFishingJoyData);
}
FishingJoyData::FishingJoyData()
{
    
}
FishingJoyData::~FishingJoyData()
{
    this->flush();
}
bool FishingJoyData::init()
{
    _isBeginner = CCUserDefault::sharedUserDefault()->getBoolForKey("beginner",true);
    if(_isBeginner == true){
        this->reset();
        this->flush();
        this->setIsBeginner(false);
    }else{
        _isBeginner = CCUserDefault::sharedUserDefault()->getBoolForKey("beginner");
        _soundVolume = CCUserDefault::sharedUserDefault()->getFloatForKey("sound");
        _musicVolume = CCUserDefault::sharedUserDefault()->getFloatForKey("music");
        _gold = CCUserDefault::sharedUserDefault()->getIntegerForKey("gold");
        CCUserDefault::sharedUserDefault()->purgeSharedUserDefault();
    }
    return true;
}
void FishingJoyData::reset()
{
    int gold = STATIC_DATA_INT("default_gold");
    this->setGold(gold);
    this->setIsBeginner(true);
    this->setMusicVolume(1);
    this->setSoundVolume(1);
    this->flush();
}
void FishingJoyData::alterGold(int delta)
{
    this->setGold(this->getGold()+delta);
}
void FishingJoyData::flush()
{
    CCUserDefault::sharedUserDefault()->setFloatForKey("sound", this->getSoundVolume());
    CCUserDefault::sharedUserDefault()->setBoolForKey("beginner", this->getIsBeginner());
    CCUserDefault::sharedUserDefault()->setIntegerForKey("gold", this->getGold());
    CCUserDefault::sharedUserDefault()->setFloatForKey("music", this->getMusicVolume());
    CCUserDefault::sharedUserDefault()->flush();
    CCUserDefault::sharedUserDefault()->purgeSharedUserDefault();
}
