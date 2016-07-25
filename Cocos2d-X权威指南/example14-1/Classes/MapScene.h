#ifndef __MAPSCENE_H__
#define __MAPSCENE_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

#include "Astar.h"
#include "AstarItem.h"

using namespace cocos2d;

class MapScene :public CCLayer
{
private:
	CCArray * path;
	int stepindex;
	int smallstepindex;
public:
	int vmove;
	int hmove;
	Astar * myastar;
	CCSprite*	m_tamara;
	virtual bool init(); 
	void update(float dt);
	//void MapScene::update1(ccTime dt);
	static cocos2d::CCScene* scene();
	CCPoint convertto2d(float x,float y);
	virtual void menuCloseCallback(CCObject* pSender);
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	LAYER_CREATE_FUNC(MapScene);
};

#endif
