//
//  Cannon.h
//  FishingJoy
//
//  Created by Ringo_D on 12-9-14.
//
//

#ifndef __FishingJoy__Cannon__
#define __FishingJoy__Cannon__
#include "cocos2d.h"
#include "Bullet.h"
typedef enum{
    k_Cannon_Type_1 = 0,
    k_Cannon_Type_2,
    k_Cannon_Type_Count,
    k_Cannon_Type_Invalid
}CannonType;
class Cannon : public cocos2d::CCNode
{
public:
    Cannon();
    ~Cannon();
    static Cannon* create(CannonType type = k_Cannon_Type_1);
    bool init(CannonType type = k_Cannon_Type_1);
    
    void aimAt(cocos2d::CCPoint target);
    CC_PROPERTY(CannonType, _type, Type);
    //获取当前的cannonSprite
    cocos2d::CCSprite* getCurCannonSprite();
protected:
    cocos2d::CCArray* _cannonSprites;
};

#endif /* defined(__FishingJoy__Cannon__) */
