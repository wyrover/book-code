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
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCTMXTiledMap *map = CCTMXTiledMap::create("iso-test-zorder.tmx");
	addChild(map, 0, 1);
    
    CCSize s = map->getContentSize();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	map->setPosition(ccp(-s.width/2 + winSize.width/2,0));
    
    setTouchEnabled(true);
    
    return true;
}
void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);
    
    CCPoint m_tBeginPos = touch->locationInView();	
    m_tBeginPos = CCDirector::sharedDirector()->convertToGL( m_tBeginPos );
    
     CCTMXTiledMap* map = (CCTMXTiledMap*) getChildByTag(1);
     CCPoint mapp = map->getPosition();
     CCPoint aimmapindex = convertto2d(m_tBeginPos.x - mapp.x,m_tBeginPos.y - mapp.y);
     if(aimmapindex.x < 0 || aimmapindex.y < 0 || aimmapindex.x >= map->getMapSize().width || aimmapindex.y >= map->getMapSize().height)
     {
     return;
     }
     CCTMXLayer* layer = map->layerNamed("grass");
     layer->setTileGID(4,aimmapindex);
}
CCPoint HelloWorld::convertto2d(float x,float y){
	CCTMXTiledMap* map = (CCTMXTiledMap*) getChildByTag(1);
	int mapWidth = map->getMapSize().width * map->getTileSize().width;
	int mapHeight = map->getMapSize().height * map->getTileSize().height;
	double distanse,sin1,sin11,sin22,cos11,cos1;
	int d2x,d2y;
	double mystatic5 = sqrt(5.0);
	double mystatic = 16 * mystatic5;
	if(x > mapWidth/2){
        distanse = sqrt((x - mapWidth/2) * (x - mapWidth/2) + (mapHeight - y) * (mapHeight - y));
        sin1 = (mapHeight - y)/distanse;
        cos1 = (x - mapWidth/2)/distanse;
        sin11 = (sin1 * 2 - cos1) / mystatic5;
        cos11 = (sin1 + cos1 * 2) / mystatic5;
        d2y = distanse * 5 / 4 * sin11 / mystatic;
        sin22 = (2 * sin1 + cos1) / mystatic5;
        d2x = distanse * 5 / 4 * sin22 / mystatic;
        return ccp(d2x,d2y);
	}else{
        distanse = sqrt((mapWidth/2 - x) * (mapWidth/2 - x) + (mapHeight - y) * (mapHeight - y));
        sin1 = (mapHeight - y)/distanse;
        cos1 = (mapWidth/2 - x)/distanse;
        sin11 = (sin1 * 2 - cos1) / mystatic5;
        cos11 = (sin1 + cos1 * 2) / mystatic5;
        d2x = distanse * 5 / 4 * sin11 / mystatic;
        //sin22 = 4.0 * cos11 / 5 + 3.0 * sin11 / 5;
        sin22 = (2 * sin1 + cos1) / mystatic5;
        d2y = distanse * 5 / 4 * sin22 / mystatic;
        return ccp(d2x,d2y);
	}
}