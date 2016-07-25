#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class HelloWorld : public cocos2d::CCLayer
{
public:
    
    virtual bool init();  

    
    static cocos2d::CCScene* scene();
    
    CCPoint convertto2d(float x,float y);
    
    void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    
    LAYER_CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
