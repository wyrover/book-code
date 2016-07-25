//
//  GameObjHero.cpp
//  example12-1
//
//  Created by shuoquan man on 12-10-14.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "GameObjHero.h"
#include "GameMainScene.h"
GameObjHero::GameObjHero(void)
{
}

GameObjHero::~GameObjHero(void)
{
}
CCRect GameObjHero::rect()
{
    CCSize s = CCSizeMake(100,125);
    return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
}
void GameObjHero::touchDelegateRetain()
{
    this->retain();
}

void GameObjHero::touchDelegateRelease()
{
    this->release();
}
void GameObjHero::onEnter()
{
    CCNode::onEnter();
    this->setContentSize(CCSizeMake(85,90));
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCSprite * obj = CCSprite::create("s_hurt.png");
    hurt = obj->getTexture();
    obj = CCSprite::create("s_jump.png");
    jump = obj->getTexture();
    mainsprite = CCSprite::create("s_1.png");
    //动画
    CCAnimation * animation = CCAnimation::create();
    animation->addSpriteFrameWithFileName("s_1.png");
    animation->addSpriteFrameWithFileName("s_2.png");
    animation->addSpriteFrameWithFileName("s_3.png");
    animation->addSpriteFrameWithFileName("s_4.png");
    animation->addSpriteFrameWithFileName("s_5.png");
    animation->addSpriteFrameWithFileName("s_6.png");
    animation->setDelayPerUnit(0.1f);
    animation->setRestoreOriginalFrame(true);
    //运行奔跑动画
    mainsprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
    state = 0;
    addChild(mainsprite);
}
void GameObjHero::setState(short var){
    if(state == var)
        return;
    state = var;
    switch(state){
        case 1://跳跃
            this->stopAllActions();
            mainsprite->stopAllActions();
            mainsprite->setTexture(jump);
            this->runAction(CCSequence::create(CCJumpBy::create(2.5,ccp(0,0),100,1),CCCallFuncN::create(this, callfuncN_selector(GameObjHero::jumpend)),NULL));
            break;
        case 2://受伤
            this->stopAllActions();
            mainsprite->stopAllActions();
            mainsprite->setTexture(hurt);
            this->runAction(CCSequence::create(CCBlink::create(3, 10),CCCallFuncN::create(this, callfuncN_selector(GameObjHero::hurtend)),NULL));
            ((GameMain *)this->getParent())->setover();
            break;
        case 0://奔跑动画
            this->stopAllActions();
            mainsprite->stopAllActions();
            CCAnimation * animation = CCAnimation::create();
            animation->addSpriteFrameWithFileName("s_1.png");
            animation->addSpriteFrameWithFileName("s_2.png");
            animation->addSpriteFrameWithFileName("s_3.png");
            animation->addSpriteFrameWithFileName("s_4.png");
            animation->addSpriteFrameWithFileName("s_5.png");
            animation->addSpriteFrameWithFileName("s_6.png");
            animation->setDelayPerUnit(0.1f);
            animation->setRestoreOriginalFrame(true);
            mainsprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
            break;
    }
}
void GameObjHero::jumpend(){
    setState(0);
}
void GameObjHero::hurtend(){
    setState(0);
}
void GameObjHero::onExit()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}    
bool GameObjHero::containsTouchLocation(CCTouch* touch)
{
    return CCRect::CCRectContainsPoint(rect(), convertTouchToNodeSpaceAR(touch));
}
bool GameObjHero::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if(((GameMain *)this->getParent())->isover)
        return false;
    //设置运动状态
    setState(1);
    return true;   
}
void GameObjHero::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
}
void GameObjHero::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
}