//
//  PanelLayer.cpp
//  FishingJoy
//
//  Created by Ringo_D on 12-9-8.
//
//

#include "PanelLayer.h"
#include "GameScene.h"
#include "StaticData.h"
USING_NS_CC;
bool PanelLayer::init()
{
    if(CCLayer::init()){
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        CCMenuItemSprite* pause = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(STATIC_DATA_STRING("pause_normal")), CCSprite::createWithSpriteFrameName(STATIC_DATA_STRING("pause_selected")),this,menu_selector(PanelLayer::pause));
        CCMenu* menu = CCMenu::create(pause, NULL);
        this->addChild(menu);
        
        CCSize pauseSize = pause->getContentSize();
        menu->setPosition(CCPointMake(winSize.width-pauseSize.width*0.5, pauseSize.height*0.5));

        return true;
    }
    return false;
}
void PanelLayer::pause(CCObject* pObj)
{
    GameScene* gameScene = (GameScene*)this->getParent();
    gameScene->pause();
}
