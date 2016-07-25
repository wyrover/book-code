//
//  GameHeroBullet.cpp
//  example11-1
//
//  Created by shuoquan man on 12-10-14.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameHeroBullet.h"

GameHeroBullet::GameHeroBullet(void)
{
    isvisable = false;
}

GameHeroBullet::~GameHeroBullet(void)
{
}
void GameHeroBullet::setIsVisable(){
    //子弹移动，运行动作
    this->setVisible(true);
    isvisable = true;
    this->runAction(CCSequence::create(CCMoveTo::create((-this->getPosition().y + 550)/150,ccp(this->getPosition().x,550)),CCCallFuncN::create(this, callfuncN_selector(GameHeroBullet::setIsNotVisable)),NULL));;
}
void GameHeroBullet::setIsNotVisable(){
    this->setVisible(false);
    isvisable = false;
    this->stopAllActions();
}
bool GameHeroBullet::getIsvisble(){
    return isvisable;
}
void GameHeroBullet::onEnter()
{
    CCNode::onEnter();
    //初始化主体
    this->setContentSize(CCSizeMake(21,52));
    CCSprite *mainbody = CCSprite::create("YuGuZD.png");
    addChild(mainbody);
}
void GameHeroBullet::onExit()
{
    CCNode::onExit();
}