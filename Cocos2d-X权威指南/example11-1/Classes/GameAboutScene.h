//
//  GameAboutScene.h
//  example11-1
//
//  Created by shuoquan man on 12-10-14.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef example15_1_GameAboutScene_h
#define example15_1_GameAboutScene_h
#include "cocos2d.h"
using namespace cocos2d;
class GameAbout : public cocos2d::CCLayer
{
public:
    
    virtual bool init();
    
    virtual void onEnter();
    
    virtual void onExit();
    
    static cocos2d::CCScene* scene();
    
    void menuBackCallback(CCObject* pSender);
    
    void menuEnter();
    
    LAYER_CREATE_FUNC(GameAbout);
};


#endif
