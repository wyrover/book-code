//
//  ScheduleCountDown.cpp
//  FishingJoy
//
//  Created by Ringo_D on 12-9-11.
//
//

#include "ScheduleCountDown.h"
USING_NS_CC;
ScheduleCountDown* ScheduleCountDown::create(ScheduleCounterDelegate* target, int perimeter, bool loop)
{
    ScheduleCountDown* scheduleCountDown = new ScheduleCountDown();
    scheduleCountDown->init(target, perimeter, loop);
    scheduleCountDown->autorelease();
    return scheduleCountDown;
}
bool ScheduleCountDown::init(ScheduleCounterDelegate* target, int perimeter, bool loop)
{
    _target = target;
    _maxTime = perimeter;
    _curTime = _maxTime;
    _loop = loop;
    this->schedule(schedule_selector(ScheduleCountDown::schedulePerSecond), 1.0f);
    return true;
}
void ScheduleCountDown::schedulePerSecond()
{
    _curTime--;
    if(_curTime <= 0){
        if(this->getLoop()){
            _curTime = _maxTime;
        }else{
            this->unschedule(schedule_selector(ScheduleCountDown::schedulePerSecond));
        }
        _target->scheduleTimeUp();
    }
    _target->setScheduleNumber(_curTime);
}