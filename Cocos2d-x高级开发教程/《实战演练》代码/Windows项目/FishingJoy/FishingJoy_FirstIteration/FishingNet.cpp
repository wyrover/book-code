//
//  FishingNet.cpp
//  FishingJoy
//
//  Created by Ringo_D on 12-9-18.
//
//

#include "FishingNet.h"
#include "StaticData.h"
USING_NS_CC;
bool FishingNet::init()
{
    _fishingNetSprite = CCSprite::createWithSpriteFrameName(STATIC_DATA_STRING("fishing_net"));
    this->addChild(_fishingNetSprite);
    return true;
}
