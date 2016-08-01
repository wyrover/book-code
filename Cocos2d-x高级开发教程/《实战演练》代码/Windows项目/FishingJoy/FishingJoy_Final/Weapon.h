//
//  Weapon.h
//  FishingJoy
//
//  Created by Ringo_D on 12-9-15.
//
//

#ifndef __FishingJoy__Weapon__
#define __FishingJoy__Weapon__
#include "cocos2d.h"
#include "Cannon.h"
#include "Bullet.h"
#include "FishingNet.h"
enum{
    k_Weapon_Status_None = 0,
    k_Weapon_Status_Bullet,
    k_Weapon_Status_FishingNet
};
class Weapon : public cocos2d::CCNode
{
public:
    static Weapon* create(CannonType type = k_Cannon_Type_1);
    bool init(CannonType type = k_Cannon_Type_1);
    
    //控制函数
    bool shootTo(cocos2d::CCPoint touchLocation);
    void aimAt(cocos2d::CCPoint target);

    cocos2d::CCPoint getCollisionPoint();
    cocos2d::CCRect getCollisionArea();
    void end();
    
    CC_SYNTHESIZE_READONLY(Cannon*, _cannon, Cannon);
    CC_SYNTHESIZE_READONLY(Bullet*, _bullet, Bullet);
    CC_SYNTHESIZE_READONLY(FishingNet*, _fishingNet, FishingNet);
    int getCannonType();
    int weaponStatus();
protected:
    //通过ParticleDesigner加载的粒子效果
    cocos2d::CCParticleSystemQuad* _particle;
};

#endif /* defined(__FishingJoy__Weapon__) */
