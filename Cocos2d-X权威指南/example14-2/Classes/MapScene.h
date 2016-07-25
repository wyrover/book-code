#ifndef __MAPSCENE_H__
#define __MAPSCENE_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;

class MapScene :public CCLayer
{
private:
	int stepindex;
	int smallstepindex;
	short enemymovetick;
	CCSize ownsize;
	CCSize othersize;
	bool isreduce;
public:
	short hmove;
	double vmove;
	CCSprite* gameplayer;
	CCSprite* enemy;  
	virtual bool init(); 
	void resetreduce(float dt);
	void playermapvcollision();
	void playermaphcollision();
	void enemytick();
	void update(float dt);
	static cocos2d::CCScene* scene();
	bool iscollision(CCSprite *mysprite,CCSprite *testsprite);
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	LAYER_CREATE_FUNC(MapScene);
};

#endif
