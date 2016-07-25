//
//  GameObjEnemy.h
//  example11-1
//
//  Created by shuoquan man on 12-10-14.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef example15_1_GameObjEnemy_h
#define example15_1_GameObjEnemy_h
#include "cocos2d.h"
using namespace cocos2d;

class GameObjEnemy : public CCNode
{
public:
    CCSprite *boom;
    CCSprite *mainbody;
    GameObjEnemy(void);
    virtual ~GameObjEnemy(void);
    void releasebullet();
    virtual void onEnter();
    virtual void onExit();
    void movestart();
    void restart();
    void setdie();
    short type;
    bool islife;
    void setType(short var);
};


#endif
