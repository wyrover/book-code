//
//  GameObjMap.cpp
//  example12-1
//
//  Created by shuoquan man on 12-10-21.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameObjMap.h"
#include "GameConst.h"
#include "GameObjStar.h"

GameObjMap::GameObjMap(){
    
}

GameObjMap::~GameObjMap(){
    
}
void GameObjMap::bg1change(){
    //运动出屏幕重设位置，运动
    CCSprite * bg = (CCSprite *)this->getChildByTag(0);
    bg->setPosition(ccp(480,320));
    bg->runAction(CCSequence::create(CCMoveBy::create(20,ccp(-960,0)),CCCallFuncN::create(this, callfuncN_selector(GameObjMap::bg1change)),NULL));
    for(int i = 0;i < 5;i ++){
        ((GameObjStar *)stars1->objectAtIndex(i))->set_visable(true);
    }
}
void GameObjMap::bg2change(){
    //运动出屏幕重设位置，运动
    CCSprite * bg = (CCSprite *)this->getChildByTag(1);
    bg->setPosition(ccp(480,320));
    bg->runAction(CCSequence::create(CCMoveBy::create(20,ccp(-960,0)),CCCallFuncN::create(this, callfuncN_selector(GameObjMap::bg2change)),NULL));
    for(int i = 0;i < 5;i ++){
        ((GameObjStar *)stars2->objectAtIndex(i))->set_visable(true);
    }
}
void GameObjMap::onEnter(){
    CCNode::onEnter();
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    this->setContentSize(CCSizeMake(960,320));
    //背景初始化
    CCSprite* bg1 = CCSprite::create("back_1.png");
    bg1->setScale(0.5);
    bg1->setAnchorPoint(ccp(0,1));
    bg1->setPosition(ccp(0, size.height) );
    this->addChild(bg1,0,0);
    CCSprite* bgdi1 = CCSprite::create("back_5.png");
    bgdi1->setAnchorPoint(ccp(0,0));
    bgdi1->setPosition(ccp(0,-124) );
    bg1->addChild(bgdi1,1);
    CCSprite* bg2 = CCSprite::create("back_1.png");
    bg2->setScale(0.5);
    bg2->setAnchorPoint(ccp(0,1));
    bg2->setPosition(ccp(size.width, size.height) );
    this->addChild(bg2,0,1);
    CCSprite* bgdi2 = CCSprite::create("back_5.png");
    bgdi2->setAnchorPoint(ccp(0,0));
    bgdi2->setPosition(ccp(0,-124) );
    bg2->addChild(bgdi2,1);
    bg1->runAction(CCSequence::create(CCMoveBy::create(10,ccp(-480,0)),CCCallFuncN::create(this, callfuncN_selector(GameObjMap::bg1change)),NULL));
    bg2->runAction(CCSequence::create(CCMoveBy::create(20,ccp(-960,0)),CCCallFuncN::create(this, callfuncN_selector(GameObjMap::bg2change)),NULL));
    stars1 = CCArray::create(5);
    stars2 = CCArray::create(5);
    for(int i = 0;i < 5;i ++){
        GameObjStar* obj = new GameObjStar();
        obj->setPosition(ccp(172 + 192 * i,350));
        stars1->addObject(obj);
        bg1->addChild(obj,3);
        obj = new GameObjStar();
        obj->setPosition(ccp(172 + 192 * i,350));
        stars2->addObject(obj);
        bg2->addChild(obj,3);
    }
    stars1->retain();
    stars2->retain();
    //星星，植物等大图素的添加
    for(int i = 0;i < 7;i ++){
        CCSprite* roaddi;
        CCSprite* plant;
        if(bg1shu[i] != -1){
           CCSprite* road;
           switch(bg1shu[i]){
            case 0:
                plant = CCSprite::create("back_2.png");
                plant->setAnchorPoint(ccp(0.5,0));
                plant->setPosition( ccp(128 * i + 64,117) );
                bg1->addChild(plant,1);
                road = CCSprite::create("road_2.png");
                road->setAnchorPoint(ccp(0,0));
                road->setPosition( ccp(128 * i,0) );
                roaddi = CCSprite::create("road_3.png");
                roaddi->setAnchorPoint(ccp(0,1));
                roaddi->setPosition( ccp(128 * i,0) );
                bg1->addChild(roaddi,1);
                break;
            case 1:
                road = CCSprite::create("road_1.png");
                road->setAnchorPoint(ccp(0,0));
                road->setPosition( ccp(26 + 128 * i,0) );
                roaddi = CCSprite::create("road_4.png");
                
                roaddi->setAnchorPoint(ccp(0,1));
                roaddi->setPosition( ccp(26 + 128 * i,0) );
                bg1->addChild(roaddi,1);
                break;
            case 2:
                plant = CCSprite::create("enemy.png");
                plant->setAnchorPoint(ccp(0.5,0));
                plant->setPosition( ccp(128 * i + 64,112) );
                bg1->addChild(plant,1);
                road = CCSprite::create("road_1.png");
                road->setFlipX(true);
                road->setAnchorPoint(ccp(0,0));
                road->setPosition( ccp(128 * i,0) );
                roaddi = CCSprite::create("road_4.png");
                roaddi->setFlipX(true);
                roaddi->setAnchorPoint(ccp(0,1));
                roaddi->setPosition( ccp(128 * i,0) );
                bg1->addChild(roaddi,1);
                break;
            case 3:
                road = CCSprite::create("road_5.png");
                road->setAnchorPoint(ccp(0,0));
                road->setPosition( ccp(128 * i,0) );
                break;
            
           }
           bg1->addChild(road,1);
        }
        if(bg2shu[i] != -1){
           CCSprite* road1;
           switch(bg2shu[i]){
            case 0:
                road1 = CCSprite::create("road_2.png");
                road1->setAnchorPoint(ccp(0,0));
                road1->setPosition( ccp(128 * i,0) );
                roaddi = CCSprite::create("road_3.png");
                
                roaddi->setAnchorPoint(ccp(0,1));
                roaddi->setPosition( ccp(128 * i,0) );
                bg2->addChild(roaddi,1);
                break;
            case 1:
                plant = CCSprite::create("back_3.png");
                plant->setAnchorPoint(ccp(0.5,0));
                plant->setPosition( ccp(128 * i + 128,117) );
                bg2->addChild(plant,1);
                road1 = CCSprite::create("road_1.png");
                road1->setAnchorPoint(ccp(0,0));
                road1->setPosition( ccp(26 + 128 * i,0) );
                roaddi = CCSprite::create("road_4.png");
                
                roaddi->setAnchorPoint(ccp(0,1));
                roaddi->setPosition( ccp(26 + 128 * i,0) );
                bg2->addChild(roaddi,1);
                break;
            case 2:
                road1 = CCSprite::create("road_1.png");
                road1->setFlipX(true);
                road1->setAnchorPoint(ccp(0,0));
                road1->setPosition( ccp(128 * i,0) );
                roaddi = CCSprite::create("road_4.png");
                roaddi->setFlipX(true);
                roaddi->setAnchorPoint(ccp(0,1));
                roaddi->setPosition( ccp(128 * i,0) );
                bg2->addChild(roaddi,1);
                break;
            case 3:
                road1 = CCSprite::create("road_5.png");
                road1->setAnchorPoint(ccp(0,0));
                road1->setPosition( ccp(128 * i,0) );
                break;
                
            }
            bg2->addChild(road1,1);
        }
    }
    state = 0;
}

void GameObjMap::onExit(){
    CCNode::onExit();
}