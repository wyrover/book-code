//
//  MenuLayer.h
//  FishingJoy
//
//  Created by Ringo_D on 12-9-8.
//
//

#ifndef __FishingJoy__MenuLayer__
#define __FishingJoy__MenuLayer__
#include "cocos2d.h"
class MenuLayer : public cocos2d::CCLayer
{
public:
    CREATE_FUNC(MenuLayer);
    bool init();
    
    //设置音乐和音效按钮的状态
    void setSoundAndMusicVolume(float soundVolume, float musicVolume);
protected:
    void resume();
    void mainMenu();
    void sound();
    void music();
    void reset();
    
    
    void createMenu();
    void createBackground();
    
    cocos2d::CCMenuItemToggle* _sound;
    cocos2d::CCMenuItemToggle* _music;

};

#endif /* defined(__FishingJoy__MenuLayer__) */
