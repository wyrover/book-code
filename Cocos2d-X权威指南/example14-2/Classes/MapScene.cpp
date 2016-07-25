#include "MapScene.h"
#include <math.h>

using namespace cocos2d;

enum 
{
	kTagTileMap = 1,
};
CCScene* MapScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MapScene *layer = MapScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MapScene::init()
{
    //地图背景层
	CCSprite *bg = CCSprite::create("background.png");
	bg->setScale(1.5f);
	addChild(bg, 0);
    //地图初始化
	CCTMXTiledMap *map = CCTMXTiledMap::create("ortho-objects.tmx");
	addChild(map, 0, kTagTileMap);
	CCSize s = map->getContentSize();
	int mapWidth = map->getMapSize().width * map->getTileSize().width;
	int mapHeight = map->getMapSize().height * map->getTileSize().height;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	map->setPosition(ccp(0,0));
    //初始化主角
	gameplayer = CCSprite::create("grossini.png");
	enemy = CCSprite::create("SpinningPeas.png");
	map->addChild(gameplayer, map->getChildren()->count() );
	map->addChild(enemy, map->getChildren()->count() );
    //初始化敌人
	enemy->setPosition(ccp(winSize.width/2 + 80,32));
	enemy->setAnchorPoint(ccp(0.5f,0));
	gameplayer->setPosition(ccp(winSize.width/2,32));
	gameplayer->setAnchorPoint(ccp(0.5f,0));
	setTouchEnabled(true);
	scheduleUpdate();
	hmove = 0;
	vmove = 0;
	enemymovetick = 0;
    //主角矩形区域大小
	ownsize = CCSizeMake(40,100);
    //敌人矩形区域大小
	othersize = CCSizeMake(32,32);
    //是否是受伤状态
	isreduce = false;
    return true;
}
void MapScene::enemytick(){
    //敌人自动在自己的区域内运行逻辑
	CCPoint enemypoint = enemy->getPosition();
	if(enemymovetick >= 0 && enemymovetick <= 20){
	   enemymovetick ++;
       enemypoint.x += 2;
	}else if(enemymovetick > 20 && enemymovetick <= 60){
	   enemymovetick ++;
       enemypoint.x -= 2;
	}else if(enemymovetick > 60 && enemymovetick <= 80){
	   enemymovetick ++;
       enemypoint.x += 2;
	}else if(enemymovetick > 80){
	   enemymovetick = 0;
	}
	enemy->setPosition(enemypoint);
}
bool MapScene::iscollision(CCSprite *mysprite,CCSprite *testsprite)
{
    //矩形区域是否碰撞
	CCPoint ownp = mysprite->getPosition();
	CCPoint otherp = testsprite->getPosition();
	if(abs(ownp.x - otherp.x) <= ownsize.width/2 + othersize.width/2 
		&& ownp.y - otherp.y <= othersize.height/2
		&& ownp.y - otherp.y >= -1 * ownsize.height/2){
		return true;
	}
	return false;
}
void MapScene::resetreduce(float dt){
	isreduce = false;
}
void MapScene::update(float dt)
{
    //主角与地图的水平和数值碰撞检测
	playermaphcollision();
	playermapvcollision();
    //设置主角位置
	CCPoint playerpoint = gameplayer->getPosition();
	playerpoint.y += vmove;
    playerpoint.x += 1 * hmove;
	enemytick();
	gameplayer->setPosition(playerpoint);
    //如果主角不是受伤状态，检测是否与敌人碰撞
	if(! isreduce && iscollision(gameplayer,enemy)){
       //设置为受伤状态
	   CCActionInterval*  action = CCBlink::actionWithDuration(5, 10);
	   gameplayer->runAction(action);
	   schedule(schedule_selector(MapScene::resetreduce), 5.0f);
	   isreduce = true;
	   hmove = 0;
	}
    //地图随主角移动逻辑
	CCTMXTiledMap* map = (CCTMXTiledMap*) getChildByTag(kTagTileMap);
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	int mapWidth = map->getMapSize().width * map->getTileSize().width;
	int mapHeight = map->getMapSize().height * map->getTileSize().height;
    //获得地图相对主角位置，并设置地图位置
	float deltax = playerpoint.x - winSize.width/2;
	float deltay = playerpoint.y - 32;
	if(-deltax > 0){
	   deltax = 0;
	}
	if(-deltax < -mapWidth + winSize.width){
	   deltax = mapWidth - winSize.width;
	}
	if(-deltay > 0){
	   deltay = 0;
	}
	if(-deltay < -mapHeight + winSize.height){
	   deltay = mapHeight - winSize.height;
	}
    map->setPosition(ccp(- deltax,-deltay));
}
void MapScene::playermaphcollision(){
    //主角与地图水平（左右）图素的碰撞检测
	CCPoint playerpoint = gameplayer->getPosition();
	CCSize palyersize = gameplayer->getContentSize();
	CCTMXTiledMap* map = (CCTMXTiledMap*) getChildByTag(kTagTileMap);
	int indexx,indexy;
	char mch[256];
	CCTMXLayer* layer = map->layerNamed("logic");
    //遍历图素块
	for(int playery = playerpoint.y - palyersize.height;playery <= playerpoint.y;playery ++){
	   //人物左边界
       indexx = (playerpoint.x - palyersize.width / 2) / map->getTileSize().width;
	   indexy = (playerpoint.y) / map->getTileSize().height;
	   CCPoint playerindex = ccp(indexx,indexy);
	   int tilegid = layer->tileGIDAt(playerindex);
	   if(tilegid > 0){
          //如果碰撞，设置主角位置
	      CCDictionary *tiledic = map->propertiesForGID(tilegid);
	      CCString *mvalue = (CCString *)tiledic->objectForKey("collion");
          int mv = mvalue->intValue();
	      if(mv == 2){
             hmove = 0;
		     playerpoint.x = (indexx + 1) * map->getTileSize().width + palyersize.width / 2;
		     gameplayer->setPosition(playerpoint);
	      }
		  return;
	   }
       //人物右边界
	   indexx = (playerpoint.x + palyersize.width / 2 - 1) / map->getTileSize().width;
	   indexy = (playerpoint.y) / map->getTileSize().height;
	   playerindex = ccp(indexx,indexy);
	   tilegid = layer->tileGIDAt(playerindex);
	   if(tilegid > 0){
          //如果碰撞，设置主角位置
          CCDictionary *tiledic = map->propertiesForGID(tilegid);
          CCString *mvalue = (CCString *)tiledic->objectForKey("collion");
          int mv = mvalue->intValue();
	      if(mv == 2){
             hmove = 0;
		     playerpoint.x = (indexx) * map->getTileSize().width - palyersize.width / 2;
		     gameplayer->setPosition(playerpoint);
	      }
		  return;
	   }
	}
}
void MapScene::playermapvcollision(){
    //主角与地图竖直（下）图素的碰撞检测
	CCPoint playerpoint = gameplayer->getPosition();
	CCSize palyersize = gameplayer->getContentSize();
	CCTMXTiledMap* map = (CCTMXTiledMap*) getChildByTag(kTagTileMap);
	int indexx,indexy;
	CCTMXLayer* layer = map->layerNamed("logic");
    //人物下边界
	indexx = (playerpoint.x) / map->getTileSize().width;
	indexy = map->getMapSize().height - (playerpoint.y) / map->getTileSize().height;
	CCPoint playerindex = ccp(indexx,indexy);
	int tilegid = layer->tileGIDAt(playerindex);
	if(tilegid > 0){
       CCDictionary *tiledic = map->propertiesForGID(tilegid);
       CCString *mvalue = (CCString *)tiledic->objectForKey("collion");
       int mv = mvalue->intValue();
	   if(mv == 1){
		  if(vmove < 0){
		     vmove = 0;
			 hmove = 0;
		     playerpoint.y = (map->getMapSize().height - indexy) * map->getTileSize().height;
		     gameplayer->setPosition(playerpoint);
		  }
		  return;
	   }
	}
	vmove -= 0.2;
}
void MapScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);

    CCPoint m_tBeginPos = touch->locationInView();	
    m_tBeginPos = CCDirector::sharedDirector()->convertToGL( m_tBeginPos );
	CCPoint playerpoint = gameplayer->getPosition();
	CCSize palyersize = gameplayer->getContentSize();
    CCTMXTiledMap* map = (CCTMXTiledMap*) getChildByTag(kTagTileMap);
	CCPoint mappoint = map->getPosition();
    //根据触摸点的位置判断人物移动方向
	if(m_tBeginPos.x < playerpoint.x - palyersize.width / 2 + mappoint.x){
	   hmove = -1;//左移
	}else if(m_tBeginPos.x > playerpoint.x + palyersize.width / 2 + mappoint.x){
	   hmove = 1;//右移
	}else{
	   hmove = 0;
	}
	if(m_tBeginPos.y > playerpoint.y + palyersize.height + mappoint.y){
	   vmove = 6;//上跳
	}else{
	   vmove = 0;
	}
}

