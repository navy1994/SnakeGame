#pragma once
#include "f:\软件\电脑软件安装包\cocos2d-2.1beta3-x-2.1.0\cocos2d-2.1beta3-x-2.1.0\cocos2dx\layers_scenes_transitions_nodes\cclayer.h"
#include "cocos2d.h"
using namespace cocos2d;
class MenuScene :
	public CCLayer
{
public:
	MenuScene(void);
	~MenuScene(void);
	 virtual bool init();  

    static cocos2d::CCScene* scene();

	//a selector starting
	void menuOnNewGame(CCObject* pSender);

	void menuOnSetGame(CCObject* pSender);
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(MenuScene);
};

