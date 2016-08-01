//
//  GameScene.h
//  FishingJoy
//
//  Created by Ringo_D on 12-9-8.
//
//

#ifndef __FishingJoy__GameScene__
#define __FishingJoy__GameScene__
#include "cocos2d.h"
#include "FishLayer.h"
#include "PanelLayer.h"
#include "MenuLayer.h"
#include "BackgroundLayer.h"
#include "CannonLayer.h"
typedef enum{
    k_Operate_Pause = 0,
    k_Operate_Resume
}OperateFlag;

class GameScene : public cocos2d::CCScene
{
public:
    CREATE_FUNC(GameScene);
    bool init();
    ~GameScene();

    //菜单页面相关的函数
    void pause();
    void resume();
    void sound();
    void music();
    void reset();
    void transToMainMenu();

protected:
    BackgroundLayer* _backgroundLayer;
    CannonLayer* _cannonLayer;
    FishLayer* _fishLayer;
    PanelLayer* _panelLayer;
    MenuLayer* _menuLayer;
    
    //暂停或恢复场景内运行的所有节点
    void operateAllSchedulerAndActions(cocos2d::CCNode* node, OperateFlag flag);

    //todo 预载入资源，实现StartScene后将其删除
    void preloadResources();
};

#endif /* defined(__FishingJoy__GameScene__) */
