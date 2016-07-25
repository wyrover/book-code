//
//  GameMenuScene.cpp
//  example12-1
//
//  Created by shuoquan man on 12-10-13.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameMenuScene.h"
#include "GameMainScene.h"
#include "GameAboutScene.h"
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
    //菜单背景
    CCSprite* bg = CCSprite::create("MainMenu.png");
    bg->setScale(0.5);
    bg->setPosition( ccp(size.width/2, size.height/2) );
    this->addChild(bg, 0,0);
    //按钮
    CCMenuItemImage *newGameItem = CCMenuItemImage::create("newGameA.png", "newGameB.png",this,menu_selector(GameMenu::menuNewGameCallback));
    newGameItem->setScale(0.5);
    newGameItem->setPosition(ccp(size.width / 2 + 40,size.height / 2 - 20));
    newGameItem->setEnabled(false);
    CCMenuItemImage *continueItem = CCMenuItemImage::create("continueA.png", "continueB.png",this,menu_selector(GameMenu::menuContinueCallback));
    continueItem->setScale(0.5);
    continueItem->setPosition(ccp(size.width / 2 + 40,size.height / 2 - 60));
    continueItem->setEnabled(false);
    CCMenuItemImage *aboutItem = CCMenuItemImage::create("aboutA.png", "aboutB.png",this,menu_selector(GameMenu::menuAboutCallback));
    aboutItem->setScale(0.5);
    aboutItem->setPosition(ccp(size.width / 2 + 40,size.height / 2 - 100));
    aboutItem->setEnabled(false);
    soundItem = CCMenuItemImage::create("sound-on-A.png", "sound-on-B.png",this,menu_selector(GameMenu::menuSoundCallback));
    soundItem->setScale(0.5);
    soundItem->setEnabled(false);
    soundItem->setPosition(ccp(40,40));
    CCMenu* mainmenu = CCMenu::create(newGameItem,continueItem,aboutItem,soundItem,NULL);
    mainmenu->setPosition(ccp(0,0));
    this->addChild(mainmenu,1,3);
    issound = true;
    //初始化声音
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic( CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("background.mp3") );
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(std::string(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("background.mp3")).c_str(), true);
    return true;
}
void GameMenu::onEnter(){
    CCLayer::onEnter();
    //入场动作
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCNode* mainmenu = this->getChildByTag(3);
    mainmenu->setScale(0);
    mainmenu->runAction(CCSequence::create(CCScaleTo::create(0.5,1),CCCallFuncN::create(this, callfuncN_selector(GameMenu::menuEnter)),NULL));
}
void GameMenu::menuEnter(){
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
    CCDirector::sharedDirector()->setDepthTest(true);
    CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(0.5,GameMain::scene(), false));
}
void GameMenu::menuContinueCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->setDepthTest(true);
    CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(0.5,GameMain::scene(), false));
}
void GameMenu::menuAboutCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->setDepthTest(true);
    CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(0.5,GameAbout::scene(), false));
}
void GameMenu::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
    CCDirector::sharedDirector()->setDepthTest(false);
}

void GameMenu::onExitTransitionDidStart()
{
    CCLayer::onExitTransitionDidStart();
}
void GameMenu::menuSoundCallback(CCObject* pSender)
{
    //设置声音
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