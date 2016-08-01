//
//  Cannon.cpp
//  FishingJoy
//
//  Created by Ringo_D on 12-9-14.
//
//

#include "Cannon.h"
#include "StaticData.h"
USING_NS_CC;
Cannon* Cannon::create(CannonType type)
{
    Cannon* cannon = new Cannon();
    cannon->init(type);
    cannon->autorelease();
    return cannon;
}
bool Cannon::init(CannonType type)
{
    _cannonSprites = CCArray::createWithCapacity(k_Cannon_Type_Count);
    _cannonSprites->retain();
    CCPoint anchorPoints = CCPointMake(0.5, 0.26);
    for(int i = k_Cannon_Type_1;i < k_Cannon_Type_Count;i++){
        CCString* fileName = CCString::createWithFormat(STATIC_DATA_STRING("cannon_level_name_format"), i+1);
        CCSprite* cannonSprite = CCSprite::createWithSpriteFrameName(fileName->getCString());
        cannonSprite->setAnchorPoint(anchorPoints);
        _cannonSprites->addObject(cannonSprite);
    }
    this->setType(type);
    return true;
}
void Cannon::setType(CannonType type)
{
    if(_type != type){
        if(type >= k_Cannon_Type_Count){
            type = k_Cannon_Type_1;
        }else if(type < k_Cannon_Type_1){
            type = (CannonType)(k_Cannon_Type_Count-1);
        }
        this->removeChildByTag(_type, false);
        CCSprite* newCannonSprite = (CCSprite*)_cannonSprites->objectAtIndex(type);
        this->addChild(newCannonSprite, 0, type);
        _type = type;
    }
}
CCSprite* Cannon::getCurCannonSprite()
{
    return (CCSprite*)_cannonSprites->objectAtIndex(_type);
}
CannonType Cannon::getType()
{
    return _type;
}
Cannon::Cannon()
{
    _type = (CannonType)k_Cannon_Type_Invalid;
}
Cannon::~Cannon()
{
    CC_SAFE_RELEASE(_cannonSprites);
}