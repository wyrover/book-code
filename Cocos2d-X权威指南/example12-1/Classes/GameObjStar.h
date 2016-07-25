//
//  GameObjStar.h
//  example12-1
//
//  Created by shuoquan man on 12-10-21.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef example16_1_GameObjStar_h
#define example16_1_GameObjStar_h
#include "cocos2d.h"
using namespace cocos2d;

class GameObjStar : public CCNode
{
public:
    GameObjStar(void);
    short state;
    virtual ~GameObjStar(void);
    virtual void onEnter();
    virtual void onExit();
    bool _visable;
    void set_visable(bool var);
    bool get_visable();
};

#endif
