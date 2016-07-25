//
//  GameAboutScene.cpp
//  example11-1
//
//  Created by shuoquan man on 12-10-14.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameAboutScene.h"
#include "GameMenuScene.h"

using namespace cocos2d;

CCScene* GameAbout::scene()
{
    CCScene *scene = CCScene::create();
    
    GameAbout *layer = GameAbout::create();
    
    scene->addChild(layer);
    
    return scene;
}
bool GameAbout::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();    
    //初始化背景
    CCSprite* bg = CCSprite::create("bg.png");
    bg->setScale(0.5);
    bg->setPosition( ccp(size.width/2, size.height/2) );
    this->addChild(bg, 0,0);
    //初始化星球
    CCSprite*bgstar = CCSprite::create("moon.png");
    bgstar->setAnchorPoint(ccp(0.5,0));
    bgstar->setScale(0.5);
    bgstar->setPosition(ccp(size.width/3 * 1, 0));
    this->addChild(bgstar,1,1);
    //初始化关于框
    CCSprite*kuang = CCSprite::create("tb.png");
    kuang->setScale(0.5);
    kuang->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(kuang,2,2);
    char inf[256];
    sprintf(inf,"name:meow war\n\nprogram:shuoquan man\n\nart design:peng xu\n\ncompany:hz books\n\n    powered by cocos2D-x");
    CCLabelTTF * myjineng = CCLabelTTF::create(inf,CCSizeMake(400,400),kCCTextAlignmentLeft, "Marker Felt", 40);
    myjineng->setAnchorPoint(ccp(0,1));
    myjineng->setColor(ccc3(200,200,200));
    myjineng->setPosition(ccp(50,600));
    kuang->addChild(myjineng);
    //初始化关于标题
    CCSprite*abouttitle = CCSprite::create("about.png");
    abouttitle->setScale(0.5);
    abouttitle->setPosition(ccp(size.width/2, size.height - 20));
    this->addChild(abouttitle,3,3);
    //初始化返回按钮
    CCMenuItemImage *back = CCMenuItemImage::create("backA.png", "backB.png",this,menu_selector(GameAbout::menuBackCallback));
    back->setScale(0.5);
    back->setPosition(ccp(size.width - 20,size.height - 20));
    back->setEnabled(false);
    CCMenu* mainmenu = CCMenu::create(back,NULL);
    mainmenu->setPosition(ccp(0,0));
    this->addChild(mainmenu,3,4);
    return true;
}
void GameAbout::menuBackCallback(CCObject* pSender){
    CCDirector::sharedDirector()->replaceScene(GameMenu::scene());
}
void GameAbout::menuEnter(){
    //菜单进入后，菜单项点击有效
    CCNode* mainmenu = this->getChildByTag(4);
    CCArray* temp = mainmenu->getChildren();
    ((CCMenuItemImage *)temp->objectAtIndex(0))->setEnabled(true);
}
void GameAbout::onExit(){
    CCLayer::onExit();
}
void GameAbout::onEnter(){
    CCLayer::onEnter();
    //界面进入时，运行菜单项进入动作
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCNode* mainmenu = this->getChildByTag(4);
    mainmenu->setPositionX(-100);
    mainmenu->runAction(CCSequence::create(CCEaseElasticIn::create(CCMoveBy::create(0.5,ccp(100,0))),CCCallFuncN::create(this, callfuncN_selector(GameAbout::menuEnter)),NULL));
    //加速度动作
    CCNode*title = this->getChildByTag(3);
    title->setPositionY(size.height + 20);
    title->runAction(CCEaseElasticIn::create(CCMoveBy::create(0.5,ccp(0,-40))));
    
    CCNode*bgstar = this->getChildByTag(1);
    bgstar->setPositionX(size.width/3 * 2);
    bgstar->runAction(CCMoveBy::create(0.5,ccp(-size.width/3,0)));
    
    CCNode*kuang = this->getChildByTag(2);
    kuang->setPositionX(-200);
    kuang->runAction(CCEaseElasticIn::create(CCMoveTo::create(0.5,ccp(size.width/2,size.height/2))));
    
}