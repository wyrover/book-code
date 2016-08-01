//
//  PersonalAudioEngine.h
//  FishingJoy
//
//  Created by Ringo_D on 12-9-27.
//
//

#ifndef __FishingJoy__PersonalAudioEngine__
#define __FishingJoy__PersonalAudioEngine__
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
class PersonalAudioEngine : public CocosDenshion::SimpleAudioEngine
{
public:
    void setBackgroundMusicVolume(float volume);
    void setEffectsVolume(float volume);
    
    static PersonalAudioEngine* sharedEngine();
    
    void purge();
    bool init();
protected:
    PersonalAudioEngine();
    ~PersonalAudioEngine();
};

#endif /* defined(__FishingJoy__PersonalAudioEngine__) */
