//
//  GameScene.h
//  example11-1
//
//  Created by shuoquan man on 12-10-14.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef example15_1_GameScene_h
#define example15_1_GameScene_h
#include "cocos2d.h"
#include "GameObjHero.h"
#include "GameObjEnemy.h"
#include "GameMark.h"
using namespace cocos2d;
class GameMain : public cocos2d::CCLayer
{
public:
    static GameMain sGameMain;
    CCSprite *blood1;
    CCSprite *blood2;
    CCSprite *blood3;
    CCSprite *bg1;
    CCSprite *bg2;
    short blood;
    CCArray *bullets;
    CCArray *enemybullets;
    void menuBackCallback();
    bool isreduce;
    bool isover;
    GameObjHero *hero;
    void setover();
    CCSprite* gameover;
    CCArray *enemys;
    virtual bool init();
    virtual void update(float time);
    static cocos2d::CCScene* scene();
    void releaseenemyBullet(int x,int y);
    void releaseheroBullet(int x,int y);
    bool isCollion(CCPoint p1,CCPoint p2,int w1,int h1,int w2,int h2);
    void setherohurt();
    void resetreduce();
    GameMark* gamemark;
    LAYER_CREATE_FUNC(GameMain);
};


#endif
