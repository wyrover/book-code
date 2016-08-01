//
//  MenuLayer.cpp
//  FishingJoy
//
//  Created by Ringo_D on 12-9-8.
//
//

#include "MenuLayer.h"
#include "GameScene.h"
USING_NS_CC;
bool MenuLayer::init()
{
    if(CCLayer::init()){
        this->createBackground();
        this->createMenu();
        return true;
    }
    return false;
}
void MenuLayer::createBackground()
{
    CCLayerColor* colorBackground = CCLayerColor::create(ccc4(0, 0, 0, 128));
    this->addChild(colorBackground);
}
void MenuLayer::createMenu()
{
    int fontSize = 32;
    CCString* fontName = CCString::create("Thonburi");
    
    CCMenuItemLabel* resume = CCMenuItemLabel::create(CCLabelTTF::create("Resume", fontName->getCString(), fontSize), this, menu_selector(MenuLayer::resume));
    
    CCMenuItemLabel* mainMenu = CCMenuItemLabel::create(CCLabelTTF::create("Main Menu", fontName->getCString(), fontSize), this, menu_selector(MenuLayer::mainMenu));
    
    CCMenuItemLabel* soundOn = CCMenuItemLabel::create(CCLabelTTF::create("Sound On", fontName->getCString(), fontSize));
    CCMenuItemLabel* soundOff = CCMenuItemLabel::create(CCLabelTTF::create("Sound Off", fontName->getCString(), fontSize));
    _sound = CCMenuItemToggle::createWithTarget(this, menu_selector(MenuLayer::sound), soundOff, soundOn, NULL);
    
    CCMenuItemLabel* musicOn = CCMenuItemLabel::create(CCLabelTTF::create("Music On", fontName->getCString(), fontSize));
    CCMenuItemLabel* musicOff = CCMenuItemLabel::create(CCLabelTTF::create("Music Off", fontName->getCString(), fontSize));
    _music = CCMenuItemToggle::createWithTarget(this, menu_selector(MenuLayer::music), musicOff, musicOn, NULL);
    
    CCMenuItemLabel* reset = CCMenuItemLabel::create(CCLabelTTF::create("Reset", fontName->getCString(), fontSize), this, menu_selector(MenuLayer::reset));
    
    CCMenu* menu = CCMenu::create(resume, mainMenu, _sound, _music, reset, NULL);
    menu->alignItemsVerticallyWithPadding(10);
    this->addChild(menu);
}
void MenuLayer::resume(cocos2d::CCObject* pSender)
{
    GameScene* gameScene = (GameScene*)this->getParent();
    gameScene->resume();
}
void MenuLayer::mainMenu(cocos2d::CCObject* pSender)
{
    GameScene* gameScene = (GameScene*)this->getParent();
    gameScene->transToMainMenu();
}
void MenuLayer::sound(cocos2d::CCObject* pSender)
{
    GameScene* gameScene = (GameScene*)this->getParent();
    gameScene->sound();
}
void MenuLayer::music(cocos2d::CCObject* pSender)
{
    GameScene* gameScene = (GameScene*)this->getParent();
    gameScene->music();
}
void MenuLayer::reset(cocos2d::CCObject* pSender)
{
    GameScene* gameScene = (GameScene*)this->getParent();
    gameScene->reset();
}
void MenuLayer::setSoundAndMusicVolume(float soundVolume, float musicVolume)
{
    bool soundFlag = soundVolume>0;
    bool musicFlag = musicVolume>0;
    _sound->setSelectedIndex(soundFlag);
    _music->setSelectedIndex(musicFlag);
}