//
//  GameAboutScene.cpp
//  example12-1
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
    
    CCSprite* bg = CCSprite::create("back_1.png");
    bg->setScale(0.6);
    bg->setPosition( ccp(size.width/2, size.height/2) );
    this->addChild(bg, 0,0);
    //关于框
    CCSprite*kuang = CCSprite::create("tb.png");
    kuang->setScale(0.5);
    kuang->setRotation(90);
    kuang->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(kuang,2,2);
    char inf[256];
    sprintf(inf,"name:loli run\n\nprogram:shuoquan man\n\nart design:zuyi li\n\ncompany:hz books\n\n    powered by cocos2D-x");
    CCLabelTTF * myjineng = CCLabelTTF::create(inf,CCSizeMake(400,400),kCCTextAlignmentLeft, "Marker Felt", 20);
    myjineng->setAnchorPoint(ccp(0,1));
    myjineng->setColor(ccc3(0,0,0));
    myjineng->setPosition(ccp(100,260));
    this->addChild(myjineng);
    //关于标签
    CCSprite*abouttitle = CCSprite::create("about.png");
    abouttitle->setScale(0.5);
    abouttitle->setPosition(ccp(size.width/2, size.height - 20));
    this->addChild(abouttitle,3,3);
    //返回
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
    CCDirector::sharedDirector()->setDepthTest(true);
    CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(0.5,GameMenu::scene(), true));
}
void GameAbout::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
    CCDirector::sharedDirector()->setDepthTest(false);
}

void GameAbout::onExitTransitionDidStart()
{
    CCLayer::onExitTransitionDidStart();
}
void GameAbout::onExit(){
    CCLayer::onExit();
}
void GameAbout::onEnter(){
    CCLayer::onEnter();
    //使按钮有效
    CCNode* mainmenu = this->getChildByTag(4);
    CCArray* temp = mainmenu->getChildren();
    ((CCMenuItemImage *)temp->objectAtIndex(0))->setEnabled(true);
}