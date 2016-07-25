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

// on "init" you need to initialize your instance
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

    pSprite = CCSprite::create("HelloWorld.png");

    pSprite->setPosition( ccp(size.width/2, size.height/2) );

    this->addChild(pSprite, 0);
    
    return true;
}
void HelloWorld::registerWithTouchDispatcher(void)
{
    CCLog("sssssssssss");
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
}

void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    if(pTouches->count() >= 2){
       CCSetIterator iter = pTouches->begin();
       CCPoint mPoint1 = ((CCTouch*)(*iter))->locationInView();
       mPoint1 = CCDirector::sharedDirector()->convertToGL(mPoint1);
       iter ++;
       CCPoint mPoint2 = ((CCTouch*)(*iter))->locationInView();
       mPoint2 = CCDirector::sharedDirector()->convertToGL(mPoint2);
        
       distance = sqrt((mPoint1.x - mPoint2.x) * (mPoint1.x - mPoint2.x) + (mPoint1.y - mPoint2.y) * (mPoint1.y - mPoint2.y)); 

       deltax = (mPoint1.x + mPoint2.x)/2 - pSprite->getPositionX();
       deltay = (mPoint1.y + mPoint2.y)/2 - pSprite->getPositionY();
    }
}
void HelloWorld::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    if(pTouches->count() >= 2){ 
        CCSetIterator iter = pTouches->begin();
        CCPoint mPoint1 = ((CCTouch*)(*iter))->locationInView();
        mPoint1 = CCDirector::sharedDirector()->convertToGL(mPoint1);
        iter ++;
        CCPoint mPoint2 = ((CCTouch*)(*iter))->locationInView();
        mPoint2 = CCDirector::sharedDirector()->convertToGL(mPoint2);
        
        float mdistance = sqrt((mPoint1.x - mPoint2.x) * (mPoint1.x - mPoint2.x) + (mPoint1.y - mPoint2.y) * (mPoint1.y - mPoint2.y)); 
        
        mscale = mdistance / distance * mscale;
        
        distance = mdistance;
        
        pSprite->setScale(mscale);
        
        float x = (mPoint1.x + mPoint2.x)/2 - deltax;
        float y = (mPoint1.y + mPoint2.y)/2 - deltax;
        pSprite->setPosition(ccp(x,y));
        deltax = (mPoint1.x + mPoint2.x)/2 - pSprite->getPositionX();
        deltay = (mPoint1.y + mPoint2.y)/2 - pSprite->getPositionY();    
    }
}
void HelloWorld::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
}
void HelloWorld::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
}
void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
