#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{

    if ( !CCLayer::init() )
    {
        return false;
    }

    //创建一个节点
    CCNode *anode = CCNode::create();
    //将节点作为子节点加入场景类中
    this->addChild(anode,0);
    
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

    
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    //将菜单作为子节点加入到我们之前定义的节点中
    anode->addChild(pMenu, 1);

    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Thonburi", 34);

    
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    
    pLabel->setPosition( ccp(size.width / 2, size.height - 20) );

    //将标签作为子节点加入到我们之前定义的节点中    
    anode->addChild(pLabel, 1);

    
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    
    pSprite->setPosition( ccp(size.width/2, size.height/2) );
    
    //将背景图片作为子节点加入到我们之前定义的节点中    
    anode->addChild(pSprite, 0);
    
    //设置节点位置
    //anode->setPosition(ccp(50,50));
    //设置缩放
    //anode->setScale(0.5);
    //设置旋转
    //anode->setPosition(ccp(200,200));    
    //anode->setRotation(90.0);
    return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
