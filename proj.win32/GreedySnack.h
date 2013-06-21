#pragma once
#include "f:\软件\电脑软件安装包\cocos2d-2.1beta3-x-2.1.0\cocos2d-2.1beta3-x-2.1.0\cocos2dx\layers_scenes_transitions_nodes\cclayer.h"
#include "MenuScene.h"
#include "cocos2d.h"
#include "resource.h"
#include "Setting.h"

#include "SimpleAudioEngine.h"
class GreedySnack :
	public CCLayer
{
public:
	GreedySnack(void);
	~GreedySnack(void);

	virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

//a selector starting
	void menuOnNewGame(CCObject* pSender);
	void menuOnSetting(CCObject* pSender);
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
	void clickUp(CCObject* pSender);
	void clickDown(CCObject* pSender);
	void clickRight(CCObject* pSender);
	void clickLeft(CCObject* pSender);
	void clickMusic(CCObject* pSender);
	int getMusic();
	void step(float dt);
	void stepAI(float dt);
	int getbeanX();
	int getbeanY();
    // implement the "static node()" method manually
	CREATE_FUNC(GreedySnack);

	void onPause(CCObject* pSender);
	void onResume(CCObject* pSender);

	void onSound(CCObject* pSender);
	void offSound(CCObject* pSender);

private:
	CCSprite* pauseBg;
	int graph[20][15], Snack[2][100],SnackAI[2][100];
	int length, head, tail, direction, beanX, beanY, x, y, level, life, speed;
	int lengthAI,directionAI,headAI,tailAI,xAI,yAI,dInput,dTemp,dRand,dTempAI;
	int sumTime,die,music;
	cocos2d::CCSprite* sprite[20][15];
	bool flag, lifeFlag;
	cocos2d::CCLabelTTF* levelTag;
	cocos2d::CCLabelTTF* timeTag;
	cocos2d::CCLabelTTF* lifeTag;
	cocos2d::CCLabelTTF* speedTag;
};

