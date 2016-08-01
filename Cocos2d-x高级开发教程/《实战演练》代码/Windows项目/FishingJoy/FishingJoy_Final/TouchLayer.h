//
//  TouchLayer.h
//  FishingJoy
//
//  Created by Ringo_D on 12-9-15.
//
//

#ifndef __FishingJoy__TouchLayer__
#define __FishingJoy__TouchLayer__
#include "cocos2d.h"

class GameScene;

class TouchLayer : public cocos2d::CCLayer
{
public:
    CREATE_FUNC(TouchLayer);
    bool init();
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    //开启、关闭触摸的接口
    void setTouchEnabled(bool flag);
protected:
    GameScene* getGameScene();
    cocos2d::CCPoint locationFromTouch(cocos2d::CCTouch *touch);

};

#endif /* defined(__FishingJoy__TouchLayer__) */
