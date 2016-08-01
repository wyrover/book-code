//
//  ProgressBar.h
//  FishingJoy
//
//  Created by Ringo_D on 12-9-30.
//
//

#ifndef __FishingJoy__ProgressBar__
#define __FishingJoy__ProgressBar__
#include "cocos2d.h"
#include "ProgressProtocol.h"
class ProgressBar : public cocos2d::CCProgressTimer
{
public:
    /**
     *	@brief
     *
     *	@param 	target 	ProgressDelegate
     *	@param 	sprite 	ProgressBarSprite
     *
     *	@return
     */
    static ProgressBar* create(ProgressDelegate* target, cocos2d::CCSprite *sprite);
    bool init(ProgressDelegate* target, cocos2d::CCSprite *sprite);
    /**
     *	@brief	    增加一个百分比
     *
     *	@param 	delta 	增加值
     */
    void progressBy(float delta);
    /**
     *	@brief	     增加到一个百分比
     *
     *	@param 	destination 	目标值
     */
    void progressTo(float destination);
    /**
     *	@brief	     设置进度条前进的速率
     *
     *	@param 	Speed 	速率
     *
     *	@return
     */
    CC_SYNTHESIZE(float, _speed, Speed);
    CC_SYNTHESIZE(ProgressDelegate*, _target, Target);
    
    /**
     *	@brief	    为进度条添加前景或背景
     *
     *	@param 	Foreground 	前景，例如进度条的当前的百分比值
     *	@param 	Background 	背景
     *
     *	@return
     */
    CC_PROPERTY(cocos2d::CCNode*, _background, Background);
    CC_PROPERTY(cocos2d::CCNode*, _foreground, Foreground);
protected:
    void updatePercentage();
    void loadingFinished();
};

#endif /* defined(__FishingJoy__ProgressBar__) */
