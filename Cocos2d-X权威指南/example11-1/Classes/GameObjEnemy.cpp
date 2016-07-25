//
//  GameObjEnemy.cpp
//  example11-1
//
//  Created by shuoquan man on 12-10-14.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameObjEnemy.h"
#include "GameScene.h"
GameObjEnemy::GameObjEnemy(void)
{
}

GameObjEnemy::~GameObjEnemy(void)
{
}
void GameObjEnemy::onEnter()
{
    CCNode::onEnter();
    this->setContentSize(CCSizeMake(99,111));
    mainbody = CCSprite::create("DrDog1.png");
    //初始化动画
    CCAnimation* animation = CCAnimation::create();
    animation->addSpriteFrameWithFileName("DrDog1.png");
    animation->addSpriteFrameWithFileName("DrDog2.png");
    animation->setDelayPerUnit(0.1f);
    animation->setRestoreOriginalFrame(true);
    mainbody->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
    addChild(mainbody);
    boom = CCSprite::create("Boom1.png");
    addChild(boom);
    boom->setVisible(false);
    islife = true;
}
void GameObjEnemy::setdie(){
    islife = false;
    mainbody->setVisible(false);
    boom->setVisible(true);
    this->stopAllActions();
    //爆炸动画
    CCAnimation* boomAnimation = CCAnimation::create();
    boomAnimation->addSpriteFrameWithFileName("Boom1.png");
    boomAnimation->addSpriteFrameWithFileName("Boom2.png");
    boomAnimation->addSpriteFrameWithFileName("Boom3.png");
    boomAnimation->addSpriteFrameWithFileName("Boom4.png");
    boomAnimation->addSpriteFrameWithFileName("Boom5.png");
    boomAnimation->setDelayPerUnit(0.1f);
    boomAnimation->setRestoreOriginalFrame(true);
    boom->runAction(CCSequence::create(CCAnimate::create(boomAnimation),CCCallFuncN::create(this, callfuncN_selector(GameObjEnemy::restart)),NULL));
}
void GameObjEnemy::releasebullet(){
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCPoint pos = this->getPosition();
    if(pos.y > 20 && pos.y < size.height && islife){
       GameMain *p = (GameMain *) this->getParent();
       p->releaseenemyBullet(pos.x,pos.y - 30);
    }
}
void GameObjEnemy::onExit()
{
    CCNode::onExit();
}
void GameObjEnemy::setType(short var){
    type = var;
}
void GameObjEnemy::restart(){
    mainbody->setVisible(true);
    boom->setVisible(false);
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    this->setPosition(ccp(size.width/4 * type,size.height + 50));
    islife = true;
    mainbody->setVisible(true);
    boom->setVisible(false);
    this->movestart();
}
void GameObjEnemy::movestart(){
    islife = true;
    int type = CCRANDOM_0_1() * 4;
    //贝塞尔曲线移动
    ccBezierConfig bezier2;
    bezier2.controlPoint_1 = CCPointMake(this->getPosition().x - 400,330);
    bezier2.controlPoint_2 = CCPointMake(this->getPosition().x + 400,280);
    bezier2.endPosition = CCPointMake(this->getPosition().x,-70);
    CCBezierTo * bezierBy2 = CCBezierTo::create(6, bezier2);
    ccBezierConfig bezier1;
    bezier1.controlPoint_1 = CCPointMake(this->getPosition().x + 400,330);
    bezier1.controlPoint_2 = CCPointMake(this->getPosition().x - 400,280);
    bezier1.endPosition = CCPointMake(this->getPosition().x,-70);
    CCBezierTo * bezierBy1 = CCBezierTo::create(6, bezier1);
    switch(type){
        case 0:
        case 1:
           this->runAction(CCSequence::create(CCMoveBy::create(6,ccp(0,-600)),CCCallFuncN::create(this, callfuncN_selector(GameObjEnemy::restart)),NULL));
            break;
        case 2:
            this->runAction(CCSequence::create(bezierBy2,CCCallFuncN::create(this, callfuncN_selector(GameObjEnemy::restart)),NULL));
            break;
        case 3:
            this->runAction(CCSequence::create(bezierBy1,CCCallFuncN::create(this, callfuncN_selector(GameObjEnemy::restart)),NULL));
            break;
        
    }
    schedule(schedule_selector(GameObjEnemy::releasebullet), 1.2f);
}