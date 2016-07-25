//
//  GameMainScene.cpp
//  example12-1
//
//  Created by shuoquan man on 12-10-21.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameMainScene.h"
#include "GameMenuScene.h"
#include "GameObjMap.h"
#include "GameConst.h"
#include "GameObjStar.h"
using namespace cocos2d;

CCScene* GameMain::scene()
{
    CCScene *scene = CCScene::create();
    
    GameMain *layer = GameMain::create();
    
    scene->addChild(layer);
    
    return scene;
}
bool GameMain::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();    
    map = new GameObjMap();
    map->setAnchorPoint(ccp(0,1));
    map->setPosition(ccp(0,size.height));
    addChild(map,0);
    hero = new GameObjHero();
    hero->setScale(0.5);
    hero->setPosition(ccp(100,160));
    addChild(hero,1);
    gamemark = new GameMark();
    addChild(gamemark,4);
    gamemark = new GameMark();
    addChild(gamemark,4);
    //游戏结束
    gameover = CCSprite::create("gameover.png");
    gameover->setAnchorPoint(ccp(0.5,0.5));
    gameover->setPosition(ccp(0,0));
    gameover->setPosition(ccp(size.width/2,size.height/2 + 70));
    gameover->setVisible(false);
    gameover->setScale(0.5);
    addChild(gameover,5);
    //菜单项
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create("back.png","back.png",
                                                          this,menu_selector(GameMain::menuBackCallback) );
    pCloseItem->setPosition( ccp(size.width/2,size.height/2 - 50) );
    pCloseItem->setScale(0.5);
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu,5,25);
    pMenu->setVisible(false);
    pMenu->setEnabled(false);
    isover = false;
    scheduleUpdate();
    return true;
}
void GameMain::setover(){
    //游戏结束
    CCMenu* pMenu = (CCMenu *)this->getChildByTag(25);
    pMenu->setVisible(true);
    pMenu->setEnabled(true);
    gameover->setVisible(true);
    gameover->setScale(0);
    pMenu->setScale(0);
    pMenu->runAction(CCScaleTo::create(0.5,1));
    gameover->runAction(CCScaleTo::create(0.5,0.5));
    isover = true;
}
void GameMain::isherodrop(){
    //判断主角是否从地图掉落
    CCPoint p1 = (map->getChildByTag(0))->getPosition();
    CCPoint p2 = (map->getChildByTag(1))->getPosition();    
    int temp;
    if(p1.x <= 100 && (p1.x + 480) >= 100){
        temp = (100 - p1.x) / 64;
        if(bg1shu[temp] == -1){
           hero->setState(2);
        }
    }else{
        temp = (100 - p2.x) / 64;
        if(bg2shu[temp] == -1){
            hero->setState(2);
        } 
    }
}
bool GameMain::isCollion(CCPoint p1,CCPoint p2,int w1,int h1,int w2,int h2){
    if(abs(p1.x - p2.x) < w1 + w2 && abs(p1.y - p2.y) < h1 + h2){
        return true;
    }
    return false;
};
void GameMain::update(float time){
    if(hero->state == 0)
       isherodrop();
    CCPoint p1 = (map->getChildByTag(0))->getPosition();
    CCPoint p2 = (map->getChildByTag(1))->getPosition();
    //主角是否吃到星星检测
    for(int i = 0;i < 5;i ++){
        if(p1.x <= 100 && (p1.x + 480) >= 100){
            GameObjStar *obj = (GameObjStar *)(map->stars1)->objectAtIndex(i);
            if(obj->get_visable() && isCollion(ccp(100,hero->getPosition().y + 62.5),ccp(p1.x + 86 + 96 * i,280),40,35,18.25,17.75)){
                obj->set_visable(false);
                gamemark->addnumber(200);
            }
        }else{
            GameObjStar *obj = (GameObjStar *)(map->stars2)->objectAtIndex(i);
            if(obj->get_visable() && isCollion(ccp(100,hero->getPosition().y + 62.5),ccp(p2.x + 86 + 96 * i,280),40,35,18.25,17.75)){
                obj->set_visable(false);
                gamemark->addnumber(200);
            }
        }
    }
}
void GameMain::menuBackCallback(CCObject* pSender){
    CCDirector::sharedDirector()->setDepthTest(true);
    CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(0.5,GameMenu::scene(), true));
}
void GameMain::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
    CCDirector::sharedDirector()->setDepthTest(false);
}

void GameMain::onExitTransitionDidStart()
{
    CCLayer::onExitTransitionDidStart();
}
void GameMain::onExit(){
    CCLayer::onExit();
}
void GameMain::onEnter(){
    CCLayer::onEnter();
}
