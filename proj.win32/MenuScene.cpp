#include "MenuScene.h"
#include "GreedySnack.h"
#include "Setting.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
using namespace CocosDenshion;

MenuScene::MenuScene(void)
{
}


MenuScene::~MenuScene(void)
{
}

CCScene* MenuScene::scene()
{
//	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Stand.mp3",true);
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        MenuScene *layer = MenuScene::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    bool bRet = false;
    do 
    {  
        CC_BREAK_IF(! CCLayer::init());

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.关闭退出按钮。

		//CCSprite* mySprite = CCSprite::spriteWithSpriteFrameName("menubg2.png");

       // CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();

                //cache->addSpriteFramesWithFile("snack.plist", "snack.png");


        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(MenuScene::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 32, 43));

		//开始游戏按钮，点击进入HelloWorld::menuOnNewGame函数。
		CCMenuItemImage *pStartItem = CCMenuItemImage::create(
			"newgameA.png", 
			"newgameB.png",
			this,
			menu_selector(MenuScene::menuOnNewGame));

		//pStartItem->setScale(0);
		pStartItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width * 0.5 - 50, 130));


		CCMenuItemImage *setGame = CCMenuItemImage::create(
			"settingA.png", 
			"settingB.png",
			this,
			menu_selector(MenuScene::menuOnSetGame));

		setGame->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width * 0.5 - 50, 60));


        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, pStartItem,setGame,NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.把pMenu添加
        this->addChild(pMenu, 1);



		CCSize size = CCDirector::sharedDirector()->getWinSize();
        
        // 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite* pSprite = CCSprite::create("menubg2.png");
        CC_BREAK_IF(! pSprite);

        // Place the sprite on the center of the screen
		//pSprite->setScale(0);
        pSprite->setPosition(ccp(size.width/2, size.height/2));

        // Add the sprite to HelloWorld layer as a child layer.
        this->addChild(pSprite, 0);

        bRet = true;
    } while (0);

    return bRet;
}

void MenuScene::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}


//点击开始后，达到这里
void MenuScene::menuOnNewGame(CCObject* pSender)
{
//	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("game.wma",music);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Cheer.mp3");
//到游戏页面
	CCScene* scene = NULL;
	do
	{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);
		GreedySnack* p = GreedySnack::create();
		CC_BREAK_IF(!p);
		scene->addChild(p);
		CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInL::create(0.5f, scene));
	} while (0);
}

void MenuScene::menuOnSetGame(CCObject* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Cheer.mp3");
//到游戏页面
	CCScene* scene = NULL;
	do
	{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);
		Setting* p = Setting::create();
		CC_BREAK_IF(!p);
		scene->addChild(p);
		CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInL::create(0.5f, scene));
	} while (0);
}