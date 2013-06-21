#include "Setting.h"
using namespace cocos2d;

Setting::Setting(void)
{
}


Setting::~Setting(void)
{
}

bool Setting::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(! CCLayer::init());
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		select = CCSprite::create("select.png");

		CCMenuItemFont *slow= CCMenuItemFont::create("  slow   ",this, menu_selector(Setting::menuSlowCallback));
		CCMenuItemFont *quick= CCMenuItemFont::create("  quick     ",this, menu_selector(Setting::menuQuickCallback));
		CCMenuItemFont *fast = CCMenuItemFont::create("  fast     ",this,menu_selector(Setting::menuFastCallback));
		CCMenuItemFont *back= CCMenuItemFont::create("  back   ",this, menu_selector(MenuScene::menuOnNewGame));

	//	CCSize size = CCDirector::sharedDirector()->getWinSize();

		slow->setPosition(ccp(size.width *0.5,size.height * 0.6));
		if(AIspeed==500)
		select->setPosition(ccp(size.width *0.6,size.height * 0.6));
		else if(AIspeed==300)
			select->setPosition(ccp(size.width *0.6,size.height * 0.4));
		else if(AIspeed==200)
			select->setPosition(ccp(size.width *0.6,size.height * 0.2));
		this->addChild(select,2);
		quick->setPosition(ccp(size.width *0.5,size.height * 0.4));

		fast->setPosition(ccp(size.width *0.5,size.height * 0.2));

		back->setPosition(ccp(size.width *0.9,size.height * 0.9));
		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu = CCMenu::create(slow,quick,fast,back,NULL);
		CC_BREAK_IF(! pMenu);
		pMenu->setPosition(CCPointZero);
		this->addChild(pMenu,1);

		CCSprite* background = CCSprite::create("snackbg1.jpg");
		background->setScale(2);
		background->setAnchorPoint(CCPointZero);
		CC_BREAK_IF(! background);
		this->addChild(background,0);
		bRet = true;
	} while (0);
	return bRet;
}

CCScene *Setting::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		Setting *layer = Setting::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}
void Setting::menuSlowCallback(CCObject* pSender)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("set.wav");
	AIspeed = 500;
	select->setPosition(ccp(size.width *0.6,size.height * 0.6));
}

void Setting::menuQuickCallback(CCObject* pSender)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("set.wav");
	AIspeed = 300;
	select->setPosition(ccp(size.width *0.6,size.height * 0.4));
}
void Setting::menuFastCallback(CCObject* pSender)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("set.wav");
	AIspeed = 200;
	select->setPosition(ccp(size.width *0.6,size.height * 0.2));
}