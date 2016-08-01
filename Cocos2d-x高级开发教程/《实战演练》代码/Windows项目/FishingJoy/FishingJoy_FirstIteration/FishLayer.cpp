//
//  FishLayer.cpp
//  FishingJoy
//
//  Created by Ringo_D on 12-9-8.
//
//

#include "FishLayer.h"
USING_NS_CC;
bool FishLayer::init()
{
    if(CCLayer::init()){
        int capacity = 50;
        _fishes = CCArray::createWithCapacity(capacity);
        CC_SAFE_RETAIN(_fishes);
        
        for(int i = 0;i < capacity;i++){
            int type = CCRANDOM_0_1()*2;
            Fish* fish = Fish::create((FishType)type);
            _fishes->addObject(fish);
        }
        this->schedule(schedule_selector(FishLayer::addFish), 3.0);
        return true;
    }
    return false;
}
void FishLayer::addFish(float dt)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    int countToAdd = CCRANDOM_0_1() * 10 + 1;
    int countHasAdded = 0;
    CCObject* iterator;
    CCARRAY_FOREACH(_fishes,  iterator){
        Fish* fish = (Fish*)iterator;
        if(fish->isRunning() == false){
            this->addChild(fish);
            //todo 后期应重设Fish产生时的随机坐标
            int randomX = CCRANDOM_0_1() * winSize.width;
            int randomY = CCRANDOM_0_1() * winSize.height;
            fish->setPosition(CCPointMake(randomX, randomY));
            countHasAdded++;
            if(countToAdd == countHasAdded){
                break;
            }
        }
    }
}
FishLayer::~FishLayer()
{
    CC_SAFE_RELEASE(_fishes);
}
