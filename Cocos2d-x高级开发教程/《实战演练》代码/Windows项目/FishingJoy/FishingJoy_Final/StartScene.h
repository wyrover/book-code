//
//  StartScene.h
//  FishingJoy
//
//  Created by Ringo_D on 12-9-8.
//
//

#ifndef __FishingJoy__StartScene__
#define __FishingJoy__StartScene__
#include "cocos2d.h"
#include "ProgressProtocol.h"
#include "ProgressBar.h"

USING_NS_CC;

class StartLayer : public cocos2d::CCLayer, public ProgressDelegate
{
public:
    static cocos2d::CCScene* scene();
    bool init();
    CREATE_FUNC(StartLayer);
    void loadingFinished();
    void progressPercentageSetter(float percentage);

protected:
    ProgressBar* _progressBar;
    cocos2d::CCLabelTTF* _progressFg;
    void transition(CCObject* pSender);
    void loading(CCObject* pObj);
    void cacheInit();
    void audioAndUserDataInit();
    void initializationCompleted();
};
#endif /* defined(__FishingJoy__StartScene__) */
