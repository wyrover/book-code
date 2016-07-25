//
//  GameScene.cpp
//  example11-1
//
//  Created by shuoquan man on 12-10-14.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "GameAboutScene.h"
#include "GameMenuScene.h"
#include "GameScene.h"
#include "GameObjHero.h"
#include "GameHeroBullet.h"
#include "GameEnemyBullet.h"

using namespace cocos2d;

CCScene* GameMain::scene()
{
    CCScene *scene = CCScene::create();
    
    GameMain *layer = GameMain::create();
    
    scene->addChild(layer);
    
    return scene;
}
void GameMain::releaseenemyBullet(int x,int y){
    //遍历子弹数组，不在使用的子弹释放
    for(int i = 0;i < enemybullets->capacity();i ++){
        if(!((GameEnemyBullet *)enemybullets->objectAtIndex(i))->getIsvisble()){
           //设置位置，并设置为显示
           ((GameEnemyBullet *)enemybullets->objectAtIndex(i))->setPosition(ccp(x,y));
           ((GameEnemyBullet *)enemybullets->objectAtIndex(i))->setIsVisable();
           break; 
        }
    }
}
void GameMain::releaseheroBullet(int x,int y){
    //遍历子弹数组，不在使用的子弹释放
    for(int i = 0;i < bullets->capacity();i ++){
        if(!((GameHeroBullet *)bullets->objectAtIndex(i))->getIsvisble()){
            //设置位置，并设置为显示
            ((GameHeroBullet *)bullets->objectAtIndex(i))->setPosition(ccp(x,y));
            ((GameHeroBullet *)bullets->objectAtIndex(i))->setIsVisable();
            break; 
        }
    }
}
bool GameMain::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();    
    //创建背景
    bg1 = CCSprite::create("bg.png");
    bg1->setScale(0.5);
    bg2 = CCSprite::create("bg.png");
    bg2->setScale(0.5);
    bg1->setAnchorPoint(ccp(0,0));
	bg2->setAnchorPoint(ccp(0,0));
	bg1->setPosition( ccp(0,0) );
	bg2->setPosition( ccp(0,size.height) );
	this->addChild(bg1, 0);
	this->addChild(bg2, 0);
    //创建主角
    hero = new GameObjHero();
    hero->setPosition(ccp(size.width/2,-50));
    hero->setScale(0.5);
    addChild(hero,2,1);
    hero->runAction(CCMoveBy::create(0.5,ccp(0,150)));
    
    //创建敌人
    enemys = CCArray::create(3);
    for(int i = 0;i < 3;i ++){
        int type = i + 1;
        GameObjEnemy* enemy = new GameObjEnemy();
        enemy->setPosition(ccp(size.width/4 * type,size.height + 50));
        enemy->setScale(0.5);
        enemy->setType(type);
        enemys->addObject(enemy);
        addChild(enemy,1);
        enemy->movestart();
    }
    enemys->retain();
    //创建血量ui
    blood = 3;
    CCSpriteBatchNode* ui = CCSpriteBatchNode::create("cat.png");
    //CCNode *ui = CCNode::create();
    blood1 = CCSprite::create(ui->getTexture());
    blood1->setPosition(ccp(20,size.height - 20));
    blood1->setScale(0.2);
    ui->addChild(blood1);
    blood2 = CCSprite::create(ui->getTexture());
    blood2->setPosition(ccp(50,size.height - 20));
    blood2->setScale(0.2);
    ui->addChild(blood2);
    blood3 = CCSprite::create(ui->getTexture());
    blood3->setPosition(ccp(80,size.height - 20));
    blood3->setScale(0.2);
    ui->addChild(blood3);
    addChild(ui,4);
    //初始化主角子弹
    bullets = CCArray::create(5);
	for(int i = 0;i < bullets->capacity();i ++){
        GameHeroBullet * mybullet = new GameHeroBullet();
        mybullet->setIsNotVisable();
        mybullet->setScale(0.5);
        bullets->addObject(mybullet);
        this->addChild(mybullet,3);
	}
    //初始化敌人子弹
	bullets->retain();
	enemybullets = CCArray::create(10);
	for(int i = 0;i < enemybullets->capacity();i ++){
        GameEnemyBullet * mybullet = new GameEnemyBullet();
        mybullet->setIsNotVisable();
        mybullet->setScale(0.5);
        enemybullets->addObject(mybullet);
        this->addChild(mybullet,3);
	}
    gamemark = new GameMark();
    addChild(gamemark,4);
	enemybullets->retain();
    //初始化游戏结束弹板及按钮
    gameover = CCSprite::create("gameover.png");
    gameover->setAnchorPoint(ccp(0.5,0.5));
    gameover->setPosition(ccp(0,0));
    gameover->setPosition(ccp(size.width/2,size.height/2 + 70));
    gameover->setVisible(false);
    gameover->setScale(0.5);
    addChild(gameover,5);
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create("back.png","back.png",
                                                          this,menu_selector(GameMain::menuBackCallback) );
    pCloseItem->setPosition( ccp(size.width/2,size.height/2 - 50) );
    pCloseItem->setScale(0.5);
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu,5,25);
    pMenu->setVisible(false);
    pMenu->setEnabled(false);
	isreduce = false;
    isover = false;
    scheduleUpdate();
    return true;
}
void GameMain::menuBackCallback(){
    CCDirector::sharedDirector()->replaceScene(GameMenu::scene());
}
bool GameMain::isCollion(CCPoint p1,CCPoint p2,int w1,int h1,int w2,int h2){
    //判断两个矩形是否碰撞
    if(abs(p1.x - p2.x) < w1 + w2 && abs(p1.y - p2.y) < h1 + h2){
        return true;
    }
    return false;
}
void GameMain::setover(){
    //设置游戏结束
    CCMenu* pMenu = (CCMenu *)this->getChildByTag(25);
    pMenu->setVisible(true);
    pMenu->setEnabled(true);
    gameover->setVisible(true);
    gameover->setScale(0);
    pMenu->setScale(0);
    pMenu->runAction(CCScaleTo::create(0.5,1));
    gameover->runAction(CCScaleTo::create(0.5,0.5));
}
void GameMain::setherohurt(){
    //主角受伤，减血
    hero->stopAllActions();
	switch(blood){
        case 3:
            blood1->setVisible(false);
            blood --;
            break;
        case 2:
            blood2->setVisible(false);
            blood --;
            break;
        case 1:
            blood3->setVisible(false);
            blood --;
            break;
        case 0:
            if(! isover){
               isover = true;
               setover();
            }
            break;
	}
	CCActionInterval*  action = CCBlink::create(5, 10);
	hero->runAction(action);
	schedule(schedule_selector(GameMain::resetreduce), 5.0f);
	isreduce = true;
}
void GameMain::resetreduce(){
    isreduce = false;
}
void GameMain::update(float time){
    //背景移动逻辑
    bg1->setPosition(ccp(bg1->getPosition().x,bg1->getPosition().y - 2));
	bg2->setPosition(ccp(bg2->getPosition().x,bg2->getPosition().y - 2));
	if(bg2->getPosition().y < 0){
        float temp = bg2->getPosition().y + 480;
        bg1->setPosition(ccp(bg2->getPosition().x,temp));
	}
	if(bg1->getPosition().y < 0){
        float temp = bg1->getPosition().y + 480;
        bg2->setPosition(ccp(bg1->getPosition().x,temp));
	}
    CCPoint hpos = hero->getPosition();
    //敌人和子弹碰撞检测
    for(int i = 0;i < 3;i ++){
        GameObjEnemy * enemy = ((GameObjEnemy *) enemys->objectAtIndex(i));
        CCPoint epos = enemy->getPosition();
        if(enemy->islife){
           for(int i = 0;i < bullets->capacity();i ++){
               if(((GameHeroBullet *)bullets->objectAtIndex(i))->getIsvisble()){
                   if(isCollion(((GameHeroBullet *)bullets->objectAtIndex(i))->getPosition(),epos,5,13,21,28)){
                       enemy->setdie();
                       gamemark->addnumber(200);
                       break;
                   }
               }
           }
        }
        if(!isreduce && enemy->islife && isCollion(hpos,epos,21,22.5,21,28)){
            enemy->setdie();
            setherohurt();
        }
    }
    //主角和敌人子弹碰撞
    if(!isreduce){
       for(int i = 0;i < enemybullets->capacity();i ++){
           if(isCollion(hpos,((GameEnemyBullet *)enemybullets->objectAtIndex(i))->getPosition(),21,22.5,5,13)){
               setherohurt();
           }
       }
    }
}