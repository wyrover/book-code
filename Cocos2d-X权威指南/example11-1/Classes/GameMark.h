//
//  GameMark.h
//  example11-1
//
//  Created by shuoquan man on 12-10-20.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef example15_1_GameMark_h
#define example15_1_GameMark_h

#include "cocos2d.h"
using namespace cocos2d;

class GameMark : public CCNode
{
public:
    GameMark(void);
    virtual ~GameMark(void);
    virtual void onEnter();
    virtual void onExit();
    CCArray *bits;
    int mark;
    void addnumber(int var);
    CCTexture2D* ui;
};

#endif
