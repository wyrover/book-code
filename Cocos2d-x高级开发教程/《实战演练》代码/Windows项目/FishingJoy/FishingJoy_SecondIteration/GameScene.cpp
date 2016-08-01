//
//  GameScene.cpp
//  FishingJoy
//
//  Created by Ringo_D on 12-9-8.
//
//

#include "GameScene.h"
#include "StaticData.h"
#include "FishingJoyData.h"
USING_NS_CC;
//todo 预载入资源，实现StartScene后将其删除
void GameScene::preloadResources()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("fishingjoy_resource.plist");
    
    int frameCount = STATIC_DATA_INT("fish_frame_count");
    for (int type = k_Fish_Type_Red; type < k_Fish_Type_Count; type++) {
        CCArray* spriteFrames = CCArray::createWithCapacity(frameCount);
        for(int i = 0;i < frameCount;i++){
            CCString* filename = CCString::createWithFormat(STATIC_DATA_STRING("fish_frame_name_format"),type,i);
            CCSpriteFrame* spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(filename->getCString());
            spriteFrames->addObject(spriteFrame);
        }
        CCAnimation* fishAnimation = CCAnimation::createWithSpriteFrames(spriteFrames);
        fishAnimation->setDelayPerUnit(STATIC_DATA_FLOAT("fish_frame_delay"));
        CCString* animationName = CCString::createWithFormat(STATIC_DATA_STRING("fish_animation"), type);
        CCAnimationCache::sharedAnimationCache()->addAnimation(fishAnimation, animationName->getCString());
    }
}
bool GameScene::init()
{
    preloadResources();
    if(CCScene::init()){
        _backgroundLayer = BackgroundLayer::create();
        this->addChild(_backgroundLayer);
        
        _fishLayer = FishLayer::create();
        this->addChild(_fishLayer);
        
        _cannonLayer = CannonLayer::create();
        this->addChild(_cannonLayer);
        
        _panelLayer = PanelLayer::create();
        this->addChild(_panelLayer);

        _menuLayer = MenuLayer::create();
        CC_SAFE_RETAIN(_menuLayer);
        
        _touchLayer = TouchLayer::create();
        this->addChild(_touchLayer);
        
        this->scheduleUpdate();
        return true;
    }
    return false;
}
void GameScene::pause()
{
    this->operateAllSchedulerAndActions(this, k_Operate_Pause);
    this->addChild(_menuLayer);
}
void GameScene::resume()
{
    this->operateAllSchedulerAndActions(this, k_Operate_Resume);
    this->removeChild(_menuLayer, false);
}
void GameScene::sound()
{
}
void GameScene::music()
{
}
void GameScene::reset()
{
}
void GameScene::transToMainMenu()
{
}
GameScene::~GameScene()
{
    CC_SAFE_RELEASE(_menuLayer);
}
void GameScene::cannonAimAt(CCPoint target)
{
    _cannonLayer->aimAt(target);
}
void GameScene::cannonShootTo(CCPoint target)
{
    _cannonLayer->shootTo(target);
}
void GameScene::update(float delta)
{
    checkOutCollision();
}
void GameScene::checkOutCollision()
{
    Weapon* weapon = _cannonLayer->getWeapon();
    if(weapon->weaponStatus() == k_Weapon_Status_Bullet){
        bool flag = this->checkOutCollisionBetweenFishesAndBullet();
        if(flag){
            this->checkOutCollisionBetweenFishesAndFishingNet();
        }
    }
}
bool GameScene::checkOutCollisionBetweenFishesAndBullet()
{
    Weapon* weapon = _cannonLayer->getWeapon();
    CCPoint bulletCollision = weapon->getCollisionPoint();
    CCArray* fishes = _fishLayer->getFishes();
    CCObject* iterator;
    CCARRAY_FOREACH(fishes, iterator){
        Fish* fish = (Fish*)iterator;
        //isRunnning判断fish是否已经在屏幕上显示
        if(fish->isRunning()){
            CCRect fishCollisionArea = fish->getCollisionArea();
            bool isCollision = fishCollisionArea.containsPoint(bulletCollision);
            if(isCollision){
                weapon->end();
                return true;
            }
        }
    }
    return false;
}
void GameScene::checkOutCollisionBetweenFishesAndFishingNet()
{
    Weapon* weapon = _cannonLayer->getWeapon();
    CCRect bulletCollision = weapon->getCollisionArea();
    CCArray* fishes = _fishLayer->getFishes();
    CCObject* iterator;
    CCARRAY_FOREACH(fishes, iterator){
        Fish* fish = (Fish*)iterator;
        //isRunnning判断fish是否已经在屏幕上显示
        if(fish->isRunning()){
            CCRect fishCollisionArea = fish->getCollisionArea();
            bool isCollision = fishCollisionArea.intersectsRect(bulletCollision);
            if(isCollision){
                this->fishWillBeCaught(fish);
            }
        }
    }
}
void GameScene::fishWillBeCaught(Fish* fish)
{
    int weaponType = _cannonLayer->getWeapon()->getCannonType();
    int fishType = fish->getType();
    float fish_percentage = STATIC_DATA_FLOAT(CCString::createWithFormat(STATIC_DATA_STRING("fish_percentage_format"),fishType)->getCString());
    float weapon_percentage = STATIC_DATA_FLOAT(CCString::createWithFormat(STATIC_DATA_STRING("weapon_percentage_format"),weaponType)->getCString());
    float percentage = weapon_percentage * fish_percentage;
    if(CCRANDOM_0_1() < percentage){
        fish->beCaught();
    }
}
void GameScene::operateAllSchedulerAndActions(cocos2d::CCNode* node, OperateFlag flag)
{
    if(node->isRunning()){
        switch (flag) {
            case k_Operate_Pause:
                node->pauseSchedulerAndActions();
                break;
            case k_Operate_Resume:
                node->resumeSchedulerAndActions();
                break;
            default:
                break;
        }
        CCArray* array = node->getChildren();
        if(array != NULL && array->count()>0){
            CCObject* iterator;
            CCARRAY_FOREACH(array, iterator){
                CCNode* child = (CCNode*)iterator;
                this->operateAllSchedulerAndActions(child, flag);
            }
        }
    }
}