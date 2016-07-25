//
//  GameMenuScene.h
//  example11-1
//
//  Created by shuoquan man on 12-10-13.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef example15_1_GameMenuScene_h
#define example15_1_GameMenuScene_h

#include "cocos2d.h"
using namespace cocos2d;
class GameMenu : public cocos2d::CCLayer
{
public:
    bool issound;
    
    CCMenuItemImage *soundItem;
    
    virtual bool init();
    
    virtual void onEnter();
    
    virtual void onExit();
    
    static cocos2d::CCScene* scene();
    
    void menuEnter();
    
    void menuNewGameCallback(CCObject* pSender);
    
    void menuContinueCallback(CCObject* pSender);
    
    void menuAboutCallback(CCObject* pSender);
    
    void menuSoundCallback(CCObject* pSender);
    
    LAYER_CREATE_FUNC(GameMenu);
};

#endif
