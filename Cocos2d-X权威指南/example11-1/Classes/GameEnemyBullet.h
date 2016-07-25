//
//  GameEnemyBullet.h
//  example11-1
//
//  Created by shuoquan man on 12-10-14.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef example15_1_GameEnemyBullet_h
#define example15_1_GameEnemyBullet_h
#include "cocos2d.h"
using namespace cocos2d;

class GameEnemyBullet : public CCNode
{
public:
    bool isvisable;
    GameEnemyBullet(void);
    virtual ~GameEnemyBullet(void);
    void setIsVisable();
    void setIsNotVisable();
    bool getIsvisble();
    virtual void onEnter();
    virtual void onExit();    
};


#endif
