//
//  Weapon.cpp
//  FishingJoy
//
//  Created by Ringo_D on 12-9-15.
//
//

#include "Weapon.h"
#include "StaticData.h"
USING_NS_CC;
Weapon* Weapon::create(CannonType type)
{
    Weapon* weapon = new Weapon();
    weapon->init(type);
    weapon->autorelease();
    return weapon;
}
bool Weapon::init(CannonType type)
{
    _cannon = Cannon::create(type);
    this->addChild(_cannon);
    
    _bullet = Bullet::create();
    _bullet->setVisible(false);
    this->addChild(_bullet);
    
    _fishingNet = FishingNet::create();
    _fishingNet->setVisible(false);
    this->addChild(_fishingNet);
    
    return true;
}