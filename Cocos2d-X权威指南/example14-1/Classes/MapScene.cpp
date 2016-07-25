#include "MapScene.h"
#include "Astar.h"
#include "AstarItem.h"
#include <math.h>

using namespace cocos2d;

enum 
{
	kTagTileMap = 1,
};
CCScene* MapScene::scene()
{
    CCScene *scene = CCScene::create();
    MapScene *layer = MapScene::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool MapScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    //初始化地图
	CCTMXTiledMap *map = CCTMXTiledMap::create("iso-test-zorder.tmx");
	addChild(map, 0, kTagTileMap);
	CCSize s = map->getContentSize();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	////----UXLOG("ContentSize: %f, %f", s.width,s.height);
	map->setPosition(ccp(-s.width/2 + winSize.width/2,0));
	//初始化人物
	m_tamara = CCSprite::create("grossinis_sister1.png");
	map->addChild(m_tamara, map->getChildren()->count() );
	m_tamara->retain();
	int mapWidth = map->getMapSize().width * map->getTileSize().width;
	int mapHeight = map->getMapSize().height * map->getTileSize().height;
	m_tamara->setPosition(ccp( mapWidth/2,112));
	m_tamara->setAnchorPoint(ccp(0.5f,0));
	setTouchEnabled(true);
	scheduleUpdate();
	vmove = -1;
	hmove = -1;
	stepindex = -1;
	myastar = new Astar();
    return true;
}
//坐标与地图位置的转换
CCPoint MapScene::convertto2d(float x,float y){
	CCTMXTiledMap* map = (CCTMXTiledMap*) getChildByTag(kTagTileMap);
	int mapWidth = map->getMapSize().width * map->getTileSize().width;
	int mapHeight = map->getMapSize().height * map->getTileSize().height;
	double distanse;
	double sin1;
	double sin11;
	double sin22;
	double cos11;
	double cos1;
	int d2x;
	int d2y;
	double mystatic5 = sqrt(5.0);//«Û∏˘∫≈5
	double mystatic = 16 * mystatic5;//–°ÕºøÈ¿‚≥§
	//char mch[256];
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
void MapScene::update(float dt)
{
	CCPoint herop = m_tamara->getPosition();
	CCPoint mapindex = convertto2d(herop.x,herop.y);
    //根据路径移动
	if(stepindex >= 1){
	   if(smallstepindex == 0){
		   int ncol = ((AstarItem *)path->objectAtIndex(stepindex))->getcol();
		   int nrow = ((AstarItem *)path->objectAtIndex(stepindex))->getrow();
		   int pcol = ((AstarItem *)path->objectAtIndex(stepindex - 1))->getcol();
		   int prow = ((AstarItem *)path->objectAtIndex(stepindex - 1))->getrow();
		   if(pcol == ncol){
			   if(prow > nrow){
			      vmove = 2;
			   }else if(prow < nrow){
                  vmove = 3;
			   }else{
			      vmove = -1;
			   }
		   }else if(prow == nrow){
			   if(pcol > ncol){
			      vmove = 1;
			   }else if(pcol < ncol){
                  vmove = 0;
			   }else{
			      vmove = -1;
			   }
		   }else{
               vmove = -1;
		   }
	   }
	   if(vmove == 0){
	      herop.x += 1;
	      herop.y -= 0.5;
	   }else if(vmove == 1){
	      herop.x -= 1;
	      herop.y += 0.5;
	   }else if(vmove == 2){
	      herop.x += 1;
	      herop.y += 0.5;
	   }else if(vmove == 3){
	      herop.x -= 1;
	      herop.y -= 0.5;
	   }else if(vmove == 4){
	      herop.x += 1;
	   }else if(vmove == 5){
	      herop.x -= 1;
	   }else if(vmove == 6){
	      herop.y += 0.5;
	   }else if(vmove == 7){
	      herop.y -= 0.5;
	   }
	   smallstepindex ++;
	   if(smallstepindex >= 32){
	      smallstepindex = 0;
		  if(stepindex >= path->count() - 1){
		     stepindex = -1;
			 vmove = -1;
		  }else{
		     stepindex ++;
			 vmove = -1;
		  }
	   }
	}
	m_tamara->setPosition(herop);
    //地图随主角移动
	CCTMXTiledMap* map = (CCTMXTiledMap*) getChildByTag(kTagTileMap);
	CCSize s = map->getContentSize();
	int mapWidth = map->getMapSize().width * map->getTileSize().width;
	int mapHeight = map->getMapSize().height * map->getTileSize().height;
	float deltax = herop.x - mapWidth/2;
	float deltay = herop.y - 112;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    map->setPosition(ccp(-s.width/2 + winSize.width/2 - deltax,-deltay));
}
void MapScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);

    CCPoint m_tBeginPos = touch->locationInView();    
    m_tBeginPos = CCDirector::sharedDirector()->convertToGL( m_tBeginPos );
    char mch[256];
	CCTMXTiledMap* map = (CCTMXTiledMap*) getChildByTag(kTagTileMap);
	CCPoint mapp = map->getPosition();
    //获得触摸点位置在地图上的索引（行列）
	CCPoint aimmapindex = convertto2d(m_tBeginPos.x - mapp.x,m_tBeginPos.y - mapp.y);
	if(aimmapindex.x < 0 || aimmapindex.y < 0 || aimmapindex.x >= map->getMapSize().width || aimmapindex.y >= map->getMapSize().height)
	{
		return;
	}
	CCPoint herop = m_tamara->getPosition();
	CCPoint mapindex = convertto2d(herop.x,herop.y);
	CCTMXLayer* layer = map->layerNamed("grass");
	int tilegid = layer->tileGIDAt(ccp(aimmapindex.x,aimmapindex.y));
	CCDictionary *tiledic = map->propertiesForGID(tilegid);
	CCString *mvalue = (CCString *)tiledic->objectForKey("conflict");
	int mv = mvalue->intValue();
	if(mv == 1){
	   return;
	}
    //A星搜索
	path = myastar->findPath(mapindex.x,mapindex.y,aimmapindex.x,aimmapindex.y,map);
	stepindex = 1;
	smallstepindex = 0;
}
void MapScene::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

