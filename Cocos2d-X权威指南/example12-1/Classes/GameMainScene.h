//
//  GameMainScene.h
//  example12-1
//
//  Created by shuoquan man on 12-10-21.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef example16_1_GameMainScene_h
#define example16_1_GameMainScene_h
#include "cocos2d.h"
#include "GameObjHero.h"
#include "GameObjMap.h"
#include "GameMark.h"
using namespace cocos2d;
class GameMain : public cocos2d::CCLayer
{
public:
    
    GameObjHero* hero;
    
    virtual bool init();
    bool isover;
    virtual void onEnter();
    
    virtual void onExit();
    
    void isherodrop();
    void setover();
    GameObjMap * map;
    GameMark *gamemark;
    CCSprite *gameover;
    static cocos2d::CCScene* scene();
    
    void menuBackCallback(CCObject* pSender);
    
    LAYER_CREATE_FUNC(GameMain);
    
    void update(float time);
    
    void onEnterTransitionDidFinish();
    void onExitTransitionDidStart();
    
    bool isCollion(CCPoint p1,CCPoint p2,int w1,int h1,int w2,int h2);
};


#endif
