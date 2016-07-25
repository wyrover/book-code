#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    CCScene *scene = CCScene::create();
    
    HelloWorld *layer = HelloWorld::create();

    
    scene->addChild(layer);
    
    return scene;
}
bool HelloWorld::init()
{
    
    if ( !CCLayer::init() )
    {
        return false;
    }

    setTouchEnabled(true);
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

    
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Thonburi", 34);

    
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    
    pLabel->setPosition( ccp(size.width / 2, size.height - 20) );

    
    this->addChild(pLabel, 1);

    
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    
    pSprite->setPosition( ccp(size.width/2, size.height/2) );

    
    this->addChild(pSprite, 0);
    
    hero = CCSprite::create("grossini.png");
    
    
    hero->setPosition( ccp(size.width/2, size.height/2) );
    
    
    this->addChild(hero, 0);
    
    return true;
}
void HelloWorld::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool HelloWorld::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint heropos = hero->getPosition();
    CCPoint location = touch->locationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    if(location.x > heropos.x - 42.5 && location.x < heropos.x + 42.5 &&
       location.y > heropos.y - 60.5 && location.y < heropos.y + 60.5){
        isControl = true;
        deltax = location.x - heropos.x;
        deltay = location.y - heropos.y;
    }
    return true;
}

void HelloWorld::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    if(isControl){
       CCPoint location = touch->locationInView();
       location = CCDirector::sharedDirector()->convertToGL(location);
    
       float x = location.x - deltax;
       float y = location.y - deltay;
       
       hero->setPosition(ccp(x,y));
    }
}

void HelloWorld::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    isControl = false;
}

void HelloWorld::ccTouchCancelled(CCTouch* touch, CCEvent* event)
{
    isControl = false;
}
void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
