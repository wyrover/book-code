//
//  Bullet.cpp
//  FishingJoy
//
//  Created by Ringo_D on 12-9-14.
//
//

#include "Bullet.h"
#include "StaticData.h"
USING_NS_CC;
enum{
    k_Bullet_Action = 0
};
bool Bullet::init()
{
    _bulletSprite = CCSprite::createWithSpriteFrameName(STATIC_DATA_STRING("bullet"));
    _bulletSprite->setAnchorPoint(CCPointMake(0.5, 1.0));
    this->addChild(_bulletSprite);
    return true;
}
void Bullet::flyTo(CCPoint targetInWorldSpace)
{
    //点的转换和角度的计算
    CCPoint startInNodeSpace = CCPointZero;
    CCPoint startInWorldSpace = this->getParent()->convertToWorldSpace(startInNodeSpace);
    CCPoint targetInNodeSpace = this->getParent()->convertToNodeSpace(targetInWorldSpace);
    float angle = ccpAngleSigned(ccpSub(targetInWorldSpace, startInWorldSpace), CCPointMake(0, 1));
    
    this->setRotation(CC_RADIANS_TO_DEGREES(angle));
    this->setPosition(startInNodeSpace);
    this->setVisible(true);
    
    //设置子弹固定的飞行速度
    float speed = 300.0f;
    float duration = ccpDistance(startInNodeSpace, targetInNodeSpace) / speed;
    CCMoveTo* moveTo = CCMoveTo::create(duration, targetInNodeSpace);
    CCCallFunc *callFunc = CCCallFunc::create(this, callfunc_selector(Bullet::end));
    
    CCFiniteTimeAction* seq = CCSequence::create(moveTo, callFunc, NULL);
    seq->setTag(k_Bullet_Action);
    this->runAction(seq);
}
void Bullet::end()
{
    this->stopActionByTag(k_Bullet_Action);
    this->setVisible(false);
}
CCPoint Bullet::getCollisionPoint()
{
    return this->getParent()->convertToWorldSpace(this->getPosition());
}
