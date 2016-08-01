//
//  Bullet.h
//  FishingJoy
//
//  Created by Ringo_D on 12-9-14.
//
//

#ifndef __FishingJoy__Bullet__
#define __FishingJoy__Bullet__
#include "cocos2d.h"
class Bullet : public cocos2d::CCNode
{
public:
    CREATE_FUNC(Bullet);
    bool init();
protected:
    cocos2d::CCSprite* _bulletSprite;
};

#endif /* defined(__FishingJoy__Bullet__) */
