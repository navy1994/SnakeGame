#pragma once
#include "f:\软件\电脑软件安装包\cocos2d-2.1beta3-x-2.1.0\cocos2d-2.1beta3-x-2.1.0\cocos2dx\layers_scenes_transitions_nodes\cclayer.h"
#include "cocos2d.h"
#include "resource.h"
#include "SimpleAudioEngine.h"
#include "MenuScene.h"

using namespace cocos2d;
class Setting :
	public CCLayer
{
public:
	Setting(void);
	~Setting(void);
	virtual bool init();  

	static cocos2d::CCScene* scene();
	void menuSlowCallback(CCObject* pSender);
	void menuQuickCallback(CCObject* pSender);
	void menuFastCallback(CCObject* pSender);

	CREATE_FUNC(Setting);
private:
	cocos2d::CCSprite* select;
};

