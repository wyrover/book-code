//
//  GameMark.cpp
//  example12-1
//
//  Created by shuoquan man on 12-10-20.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameMark.h"
GameMark::GameMark(void)
{
}

GameMark::~GameMark(void)
{
}
void GameMark::onExit()
{
    CCNode::onExit();
}
void GameMark::onEnter()
{
    CCNode::onEnter();
    CCSize size = CCDirector::sharedDirector()->getWinSize(); 
    this->setContentSize(size);
    bits = CCArray::create(5);
    //初始化标题
    CCSprite *title = CCSprite::create("score.png");
    title->setPosition(ccp(size.width/2 + 120,size.height - 15));
    title->setScale(0.5);
    addChild(title);
    //初始化分数
    for(int i = 0;i < 5;i ++){
        CCSprite * shu = CCSprite::create("shu.png");
        ui = shu->getTexture();
        shu->setScale(0.5);
        shu->setTextureRect(CCRectMake(234,0,26,31));
        shu->setPosition(ccp(size.width - 15 - i * 15,size.height - 15));
        bits->addObject(shu);
        addChild(shu);
    }
    bits->retain();
    mark = 0;
}
void GameMark::addnumber(int var){
    //按位设置数字
    mark += var;
    int temp = mark % 10;
    if(temp > 0){
        ((CCSprite *)bits->objectAtIndex(0))->setTexture(ui);
        ((CCSprite *)bits->objectAtIndex(0))->setTextureRect(CCRectMake((temp - 1) * 26,0,26,31)); 
    }else{
        ((CCSprite *)bits->objectAtIndex(0))->setTexture(ui);
        ((CCSprite *)bits->objectAtIndex(0))->setTextureRect(CCRectMake(234,0,26,31)); 
    }
    temp = (mark % 100) / 10;
    if(temp > 0){
        ((CCSprite *)bits->objectAtIndex(0))->setTexture(ui);
        ((CCSprite *)bits->objectAtIndex(1))->setTextureRect(CCRectMake((temp - 1) * 26,0,26,31));  
 
    }else{
        ((CCSprite *)bits->objectAtIndex(0))->setTexture(ui);
        ((CCSprite *)bits->objectAtIndex(1))->setTextureRect(CCRectMake(234,0,26,31)); 
    }
    temp = (mark % 1000) / 100;
    if(temp > 0){
        ((CCSprite *)bits->objectAtIndex(0))->setTexture(ui);
        ((CCSprite *)bits->objectAtIndex(2))->setTextureRect(CCRectMake((temp - 1) * 26,0,26,31)); 
 
    }else{
        ((CCSprite *)bits->objectAtIndex(0))->setTexture(ui);
        ((CCSprite *)bits->objectAtIndex(2))->setTextureRect(CCRectMake(234,0,26,31));
    }
    temp = (mark % 10000) / 1000;
    if(temp > 0){
        ((CCSprite *)bits->objectAtIndex(0))->setTexture(ui);
        ((CCSprite *)bits->objectAtIndex(3))->setTextureRect(CCRectMake((temp - 1) * 26,0,26,31)); 
 
    }else{
        ((CCSprite *)bits->objectAtIndex(0))->setTexture(ui);
        ((CCSprite *)bits->objectAtIndex(3))->setTextureRect(CCRectMake(234,0,26,31)); 
    }
    temp = mark / 10000;
    if(temp > 0){
        ((CCSprite *)bits->objectAtIndex(0))->setTexture(ui);
        ((CCSprite *)bits->objectAtIndex(4))->setTextureRect(CCRectMake((temp - 1) * 26,0,26,31));  
 
    }else{
        ((CCSprite *)bits->objectAtIndex(0))->setTexture(ui);
        ((CCSprite *)bits->objectAtIndex(4))->setTextureRect(CCRectMake(234,0,26,31));
    }
}