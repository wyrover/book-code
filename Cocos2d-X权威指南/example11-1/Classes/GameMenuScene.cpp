//
//  GameMenuScene.cpp
//  example11-1
//
//  Created by shuoquan man on 12-10-13.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameMenuScene.h"
#include "GameAboutScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* GameMenu::scene()
{
    CCScene *scene = CCScene::create();
    
    GameMenu *layer = GameMenu::create();
    
    scene->addChild(layer);
    
    return scene;
}
bool GameMenu::init()
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
    //初始化背景星球
    CCSprite*bgstar = CCSprite::create("moon.png");
    bgstar->setAnchorPoint(ccp(0.5,0));
    bgstar->setScale(0.5);
    bgstar->setPosition(ccp(size.width/3 * 2, 0));
    this->addChild(bgstar,1,1);
    //初始化标题
    CCNode *title = CCNode::create();
    title->setContentSize(CCSizeMake(size.width - 40,50));
    CCSprite *ptmLabel = CCSprite::create("meowstar.png");
    ptmLabel->setScale(0.5);
    ptmLabel->setPosition( ccp(0,30) );
    title->addChild(ptmLabel);
    CCSprite *ptbLabel = CCSprite::create("battle.png");
    ptbLabel->setScale(0.5);
    ptbLabel->setPosition( ccp(0,-30) );
    title->addChild(ptbLabel);
    title->setPosition(ccp(size.width / 2, size.height - 80));
    this->addChild(title,2,2);
    //初始化按钮
    CCMenuItemImage *newGameItem = CCMenuItemImage::create("newGameA.png", "newGameB.png",this,menu_selector(GameMenu::menuNewGameCallback));
    newGameItem->setScale(0.5);
    newGameItem->setPosition(ccp(size.width / 2,size.height / 2 - 20));
    newGameItem->setEnabled(false);
    CCMenuItemImage *continueItem = CCMenuItemImage::create("continueA.png", "continueB.png",this,menu_selector(GameMenu::menuContinueCallback));
    continueItem->setScale(0.5);
    continueItem->setPosition(ccp(size.width / 2,size.height / 2 - 80));
    continueItem->setEnabled(false);
    CCMenuItemImage *aboutItem = CCMenuItemImage::create("aboutA.png", "aboutB.png",this,menu_selector(GameMenu::menuAboutCallback));
    aboutItem->setScale(0.5);
    aboutItem->setPosition(ccp(size.width / 2,size.height / 2 - 140));
    aboutItem->setEnabled(false);
    soundItem = CCMenuItemImage::create("sound-on-A.png", "sound-on-B.png",this,menu_selector(GameMenu::menuSoundCallback));
    soundItem->setScale(0.5);
    soundItem->setEnabled(false);
    soundItem->setPosition(ccp(40,40));
    //使用按钮创建菜单
    CCMenu* mainmenu = CCMenu::create(newGameItem,continueItem,aboutItem,soundItem,NULL);
    mainmenu->setPosition(ccp(0,0));
    this->addChild(mainmenu,1,3);
    //初始化声音部分
    issound = true;//是否开启声音参数
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic( CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("background.mp3") );
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("background.mp3")).c_str(), true);
    return true;
}
void GameMenu::onEnter(){
    CCLayer::onEnter();
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    //界面进入时，运行菜单项进入动作
    CCNode* mainmenu = this->getChildByTag(3);
    mainmenu->setPositionX(-200);
    mainmenu->runAction(CCSequence::create(CCEaseElasticIn::create(CCMoveTo::create(0.5,ccp(0,0))),CCCallFuncN::create(this, callfuncN_selector(GameMenu::menuEnter)),NULL));
    
    CCNode*title = this->getChildByTag(2);
    title->setPositionY(size.height + 20);
    title->runAction(CCEaseElasticIn::create(CCMoveBy::create(0.5,ccp(0,-100))));
    
    CCNode*bgstar = this->getChildByTag(1);
    bgstar->setPositionX(size.width/3);
    bgstar->runAction(CCMoveBy::create(0.5,ccp(size.width/3,0)));
    
}
void GameMenu::menuEnter(){
    //菜单进入后，菜单项点击有效
    CCNode* mainmenu = this->getChildByTag(3);
    CCArray* temp = mainmenu->getChildren();
    for(int i = 0;i < temp->count();i ++)
        ((CCMenuItemImage *)temp->objectAtIndex(i))->setEnabled(true);
}
void GameMenu::onExit(){
    CCLayer::onExit();
}
void GameMenu::menuNewGameCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(GameMain::scene());
}
void GameMenu::menuContinueCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(GameMain::scene());
}
void GameMenu::menuAboutCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(GameAbout::scene());
}
void GameMenu::menuSoundCallback(CCObject* pSender)
{
    if(! issound){
        soundItem->setNormalImage(CCSprite::create("sound-on-A.png"));
        soundItem->setDisabledImage(CCSprite::create("sound-on-B.png"));
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("background.mp3")).c_str(), true);
       issound = true;
    }else{
        soundItem->setNormalImage(CCSprite::create("sound-off-A.png"));
        soundItem->setDisabledImage(CCSprite::create("sound-off-B.png"));
         SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
       issound = false;
    }
}