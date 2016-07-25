//
//  GameEnemyBullet.cpp
//  example11-1
//
//  Created by shuoquan man on 12-10-14.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameEnemyBullet.h"

GameEnemyBullet::GameEnemyBullet(void)
{
    isvisable = false;
}

GameEnemyBullet::~GameEnemyBullet(void)
{
}
void GameEnemyBullet::onEnter()
{
    CCNode::onEnter();
    //初始化主体
    this->setContentSize(CCSizeMake(21,52));
    CCSprite *mainbody = CCSprite::create("DrDogZD.png");
    mainbody->runAction(CCRepeatForever::create(CCRotateBy::create(1,360)));
    addChild(mainbody);
}
bool GameEnemyBullet::getIsvisble(){
    return isvisable;
}
void GameEnemyBullet::onExit()
{
    CCNode::onExit();
}
void GameEnemyBullet::setIsVisable(){
    //运行动作
    this->setVisible(true);
    isvisable = true;
    this->runAction(CCRepeatForever::create(CCRotateBy::create(1,360)));
    this->runAction(CCSequence::create(CCMoveTo::create((this->getPosition().y + 50)/150,ccp(this->getPosition().x,-50)),CCCallFuncN::create(this, callfuncN_selector(GameEnemyBullet::setIsNotVisable)),NULL));;
}
void GameEnemyBullet::setIsNotVisable(){
    this->setVisible(false);
    isvisable = false;
    this->stopAllActions();
}