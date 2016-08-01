//
//  FishingNet.h
//  FishingJoy
//
//  Created by Ringo_D on 12-9-18.
//
//

#ifndef __FishingJoy__FishingNet__
#define __FishingJoy__FishingNet__
#include "cocos2d.h"
class FishingNet : public cocos2d::CCNode
{
public:
    CREATE_FUNC(FishingNet);
    bool init();
protected:
    cocos2d::CCSprite *_fishingNetSprite;
};
#endif /* defined(__FishingJoy__FishingNet__) */
