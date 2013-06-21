#include "GreedySnack.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
using namespace cocos2d;
GreedySnack::GreedySnack(void)
{
}


GreedySnack::~GreedySnack(void)
{
}

CCScene* GreedySnack::scene()
{
    CCScene * scene = NULL;
    do
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
		GreedySnack *layer = GreedySnack::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GreedySnack::init()/////////////////////////init
{
//	music = 0;显示各种按钮，界面
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background.mp3",true);
    bool bRet = false;
    do 
    {

        CC_BREAK_IF(! CCLayer::init());

		memset(graph, 0, sizeof(graph));
//墙壁
		for (int i = 0; i < 15; i++)
		{
			graph[0][i] = graph[19][i] = 1;   // 1 = wall
		}
		for (int i = 1; i < 20; i++)
		{
			graph[i][0] = graph[i][14] = 1;
		} 


		for (int i = 1; i <= 3; i++)//1 = wall   0 = grid
			graph[1][i] = 2;      //2 = 蛇身
		graph[1][4] = 3;          //3 = 蛇头

		for (int i = 0; i < 4; i++)
		{
			Snack[0][i] = 1;
			Snack[1][i] = i + 1;
		}                         //记录贪吃蛇所在的坐标
		
		head = 3, tail = 0, length = 4;

		srand(time(0));
		do
		{
			beanX = rand() % 8 + 1;
			beanY = rand() % 8 + 1;
		} while (graph[beanX][beanY]);
		graph[beanX][beanY] = 2;         //随机产生食物

		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 15; j++)//通过规则把地图和蛇显示出来
			{
				if (graph[i][j] == 1)
					sprite[i][j] = CCSprite::create("wall.png");
				else if (graph[i][j] == 2)
					sprite[i][j] = CCSprite::create("tail.png");
				else if (graph[i][j] == 3)
					sprite[i][j] = CCSprite::create("head.jpg");
				else
					sprite[i][j] = CCSprite::create("grid.jpg");
				sprite[i][j]->setPosition(ccp(32 * i + 16, 32 * j + 16));
				this->addChild(sprite[i][j], 1);
			}
		}

		//方向默认向上，自动前进速度默认为800ms，等级为1
		life = 1, direction = 1;
		dInput = -1; dTemp = 1; level = 1, speed = 10 / (gameSpeed / 1000),gameSpeed = 500;
		dRand=-1,die=0;
		flag = true, lifeFlag = true;
		sumTime = 0;
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		char life_str[30], level_str[30], time_str[30], speed_str[30];
		sprintf(life_str, "Life: %d", life);
		lifeTag = CCLabelTTF::create(life_str, "font09.fnt", 15);//显示血量
		CC_BREAK_IF(!lifeTag);
		lifeTag->setPosition(ccp(size.width-150, 140));//life
		this->addChild(lifeTag, 1);
		sprintf(level_str, "Level: %d", level);//显示level
		levelTag = CCLabelTTF::create(level_str, "font09.fnt", 15);
		CC_BREAK_IF(!levelTag);
		levelTag->setPosition(ccp(size.width-80, 160));//level
		this->addChild(levelTag, 1);
		sprintf(time_str, "Time: %d s", sumTime / 1000);//显示time
		timeTag = CCLabelTTF::create(time_str, "font09.fnt", 15);
		CC_BREAK_IF(!levelTag);
		timeTag->setPosition(ccp(size.width-50, 140));//time
		this->addChild(timeTag, 1);
		sprintf(speed_str, "Speed: %d", speed);//显示speed
		speedTag = CCLabelTTF::create(speed_str, "font09.fnt", 15);
		CC_BREAK_IF(!speedTag);
		speedTag->setPosition(ccp(size.width-70, 180));//speed
		this->addChild(speedTag, 1);

		this->schedule(schedule_selector(GreedySnack::step), gameSpeed / 1000);
		

		CCMenuItemImage *up = CCMenuItemImage::create(//游戏中的四个按钮
		"up.jpg",
		"up1.jpg",
		this,
		menu_selector(GreedySnack::clickUp));
		CC_BREAK_IF(! up);
		up->setPosition(ccp(size.width-48*2,size.height-16*2-100));
		CCMenuItemImage *down = CCMenuItemImage::create(
		"down.jpg",
		"down1.jpg",
		this,
		menu_selector(GreedySnack::clickDown));
		CC_BREAK_IF(! down);
		down->setPosition(ccp(size.width-48*2,size.height-76*2-100));
		CCMenuItemImage *right = CCMenuItemImage::create(
		"right.jpg",
		"right1.jpg",
		this,
		menu_selector(GreedySnack::clickRight));
		CC_BREAK_IF(! right);
		right->setPosition(ccp(size.width-16*2,size.height-48*2-100));
		CCMenuItemImage *left = CCMenuItemImage::create(
		"left.jpg",
		"left1.jpg",
		this,
		menu_selector(GreedySnack::clickLeft));
		CC_BREAK_IF(! left);
		left->setPosition(ccp(size.width-76*2,size.height-48*2-100));

		CCMenuItemImage *set = CCMenuItemImage::create(
		"settingA.png",
		"settingB.png",
		this,
		menu_selector(GreedySnack::menuOnSetting));
		CC_BREAK_IF(! set);
		set->setScale(0.7);
		set->setPosition(ccp(size.width-52*2,size.height-30));

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.关闭按钮
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
			menu_selector(GreedySnack::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.//点击进入GreedySnack::menuOnNewGame
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 32, 20));//返回
		CCMenuItemImage *pReturnItem = CCMenuItemImage::create(
			"newgameA.png", 
			"newGameB.png",
			this,
			menu_selector(MenuScene::menuOnNewGame));
		CC_BREAK_IF(! pReturnItem);
		pReturnItem->setScale(0.7);
		pReturnItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 52*2, size.height-70));

		CCMenuItemImage *pStartItem = CCMenuItemImage::create(
			"backA.png", 
			"backB.png",
			this,
			menu_selector(GreedySnack::menuOnNewGame));
		CC_BREAK_IF(! pStartItem);
		pStartItem->setScale(0.7);
		pStartItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width -52*2, 100));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(up, set,down, right, left, pCloseItem, pReturnItem,pStartItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

        // Create a label and initialize with string "Hello World".

        // 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite* pSprite = CCSprite::create("leftWelcome.jpg");
		pSprite->setOpacity(90);
        CC_BREAK_IF(! pSprite);

//		CCSize size = CCDirector::sharedDirector()->getWinSize();
        // Place the sprite on the center of the screen
        pSprite->setPosition(ccp(size.width/2, size.height/2));

        // Add the sprite to HelloWorld layer as a child layer.
        this->addChild(pSprite, 0);

		CCMenuItemImage *pauseBtn = CCMenuItemImage::create(
			"startA.png", 
			"startB.png",
			this,
			menu_selector(GreedySnack::onPause));
		pauseBtn->setScale(0.7);
		pauseBtn->setPosition(ccp(size.width-52*2,60));

		CCMenuItemImage *offsd = CCMenuItemImage::create(
			"sound-on-A.png", 
			"sound-on-B.png",
			this,
			menu_selector(GreedySnack::offSound));
		offsd->setPosition(ccp(size.width-48*2,size.height-192));
		CCMenu* menu = CCMenu::create(pauseBtn,offsd,NULL);
		menu->setPosition(CCPointZero);
		this->addChild(menu);
        bRet = true;
    } while (0);

    return bRet;
}

void GreedySnack::onSound(CCObject* pSender)
{
	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->resumeAllEffects();
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCMenuItemImage *item = CCMenuItemImage::create(
			"sound-on-A.png", 
			"sound-on-B.png",
			this,
			menu_selector(GreedySnack::offSound));
		item->setPosition(ccp(size.width-48*2,size.height-192));

	CCMenu* childMenu = CCMenu::create(item,NULL);
	childMenu->setPosition(CCPointZero);
	this->addChild(childMenu);
}
void GreedySnack::offSound(CCObject* pSender)
{
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->pauseAllEffects();

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCMenuItemImage *item = CCMenuItemImage::create(
			"sound-off-A.png", 
			"sound-off-B.png",
			this,
			menu_selector(GreedySnack::onSound));
		item->setPosition(ccp(size.width-48*2,size.height-192));

	CCMenu* childMenu = CCMenu::create(item,NULL);
	childMenu->setPosition(CCPointZero);
	this->addChild(childMenu);
}

void GreedySnack::onPause(CCObject* pSender)
{
	CCDirector::sharedDirector()->pause();
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	
	CCMenuItemImage *item = CCMenuItemImage::create(
			"stopA.png", 
			"stopB.png",
			this,
			menu_selector(GreedySnack::onResume));
		item->setScale(0.7);
		item->setPosition(ccp(size.width-52*2,60));

	CCMenu* childMenu = CCMenu::create(item,NULL);
	childMenu->setPosition(CCPointZero);
	this->addChild(childMenu);
	
}

void GreedySnack::onResume(CCObject* pSender)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCDirector::sharedDirector()->resume();

	CCMenuItemImage *item = CCMenuItemImage::create(
			"startA.png", 
			"startB.png",
			this,
			menu_selector(GreedySnack::onPause));
		item->setScale(0.7);
		item->setPosition(ccp(size.width-52*2,60));

	CCMenu* childMenu = CCMenu::create(item,NULL);
	childMenu->setPosition(CCPointZero);
	this->addChild(childMenu);

}

//////////////////////////////////////////////////////////////////////////init

void GreedySnack::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

void GreedySnack::menuOnNewGame(CCObject* pSender)
{
//	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("game.wma",music);
	//点击即进入主界面。helloworld
	CCScene* scene = NULL;
	do
	{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);
		MenuScene* pScene = MenuScene::create();
		CC_BREAK_IF(!pScene);
		scene->addChild(pScene);
		CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(0.5f, scene));
	} while (0);
}

void GreedySnack::clickUp(CCObject* pSender)//方向
{
	if (flag)
		dInput = 1;
}

void GreedySnack::clickDown(CCObject* pSender)
{
	if (flag)
		dInput = 2;
}

void GreedySnack::clickRight(CCObject* pSender)
{
	if (flag)
		dInput = 3;
}

void GreedySnack::clickLeft(CCObject* pSender)
{
	if (flag)
		dInput = 4;
}

void GreedySnack::clickMusic(CCObject* pSender)
{
	music = music * -1 + 1;
}

void GreedySnack::step(float dt)
{
//	stepAI(1);

	if (flag)
		sumTime += gameSpeed;
	this->removeChild(timeTag, true);
	char time_str[30];
	sprintf(time_str, "Time: %d s", sumTime / 1000);
	timeTag = CCLabelTTF::create(time_str, "font09.fnt", 15);
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	timeTag->setPosition(ccp(size.width-50, 140));
	this->addChild(timeTag, 1);
	if(dInput != -1)//如果有输入，对未来的位置进行试验
	switch (dInput)
	{
	//向上
	case 1: x = Snack[0][head], y = Snack[1][head] + 1, lifeFlag = true;
		break;
	//向下
	case 2: x = Snack[0][head], y = Snack[1][head] - 1, lifeFlag = true;
		break;
	//向右
	case 3: x = Snack[0][head] + 1, y = Snack[1][head], lifeFlag = true;
		break;
	//向左
	case 4: x = Snack[0][head] - 1, y = Snack[1][head], lifeFlag = true;
		break;
	default: x = Snack[0][head], y = Snack[1][head], lifeFlag = false;
		break;
	}
	else//如果没输入
	{ x = Snack[0][head], y = Snack[1][head] + 1, lifeFlag = true;dInput = direction;}
	if (x == beanX && y == beanY)
	{
		;
	}
	else if(graph[x][y] == 1 )//如果下一步是墙wall
	{
		if (life && lifeFlag && direction != 0)
			life--;
		this->removeChild(lifeTag, true);
		char life_str[30];
		sprintf(life_str, "Life: %d", life);
		lifeTag = CCLabelTTF::create(life_str, "font09.fnt", 15);
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		lifeTag->setPosition(ccp(size.width-150, 140));
		this->addChild(lifeTag, 1);
		if(direction != 0)
			dTemp = direction;
		direction = 0;
		if (!life)
		{
							for (int i = 0; i < 20; i++)//根据规则显示
					{
						for (int j = 0; j < 15; j++)
						{
							if (graph[i][j] == 1)
							sprite[i][j] = CCSprite::create("wall.png");
							else if (graph[i][j] == 2)
							sprite[i][j] = CCSprite::create("tail.png");
							else if (graph[i][j] == 3)
							sprite[i][j] = CCSprite::create("head.jpg");
							else
							sprite[i][j] = CCSprite::create("grid.jpg");
							sprite[i][j]->setPosition(ccp(32 * i + 16, 32 * j + 16));
							this->addChild(sprite[i][j], 1);
						}
					}

			if(flag)
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("GameOver.mp3");
			flag = false;
			CCLabelTTF* pLabel = CCLabelTTF::create("Game Over!", "font09.fnt", 24);

			// Get window size and place the label upper. 
			CCSize size = CCDirector::sharedDirector()->getWinSize();
			pLabel->setPosition(ccp(size.width/2, size.height/2));

			// Add the label to HelloWorld layer as a child layer.
			this->addChild(pLabel, 1);
		}
		return ;
	}
	else if(graph[x][y] == 2 )//如果下一步是身子
	{
		int ca=0;
		for (int k=0;k<100;k++)
		{
			if(Snack[0][head-1]==x&&Snack[1][head-1]==y)
				 ca =1;
		}
		if ( ca)//往后的话，忽视
			//if ( (dInput+direction)==7 || ((dInput+direction)==3&&direction!=0) )//往后的话，忽视
		{
			if(dInput==1 || dInput == 3)
				dInput++;
			else
				dInput--;
		}
		else
		{
			if (life && lifeFlag)
				life--;
			this->removeChild(lifeTag, true);
			char life_str[30];
			sprintf(life_str, "Life: %d", life);
			lifeTag = CCLabelTTF::create(life_str, "font09.fnt", 15);
			CCSize size = CCDirector::sharedDirector()->getWinSize();
			lifeTag->setPosition(ccp(size.width-150, 140));
			this->addChild(lifeTag, 1);
			if(direction != 0)
				dTemp = direction;
			direction = 0;
			if (!life)
			{

							for (int i = 0; i < 20; i++)//根据规则显示
								{
									for (int j = 0; j < 15; j++)
									{
										if (graph[i][j] == 1)
										sprite[i][j] = CCSprite::create("wall.png");
										else if (graph[i][j] == 2)
										sprite[i][j] = CCSprite::create("tail.png");
										else if (graph[i][j] == 3)
										sprite[i][j] = CCSprite::create("head.jpg");
										else
										sprite[i][j] = CCSprite::create("grid.jpg");
										sprite[i][j]->setPosition(ccp(32 * i + 16, 32 * j + 16));
										this->addChild(sprite[i][j], 1);
									}
								}

				flag = false;
				CCLabelTTF* pLabel = CCLabelTTF::create("Game Over!", "font09.fnt", 24);

				// Get window size and place the label upper. 
				CCSize size = CCDirector::sharedDirector()->getWinSize();
				pLabel->setPosition(ccp(size.width/2, size.height/2));

				// Add the label to HelloWorld layer as a child layer.
				this->addChild(pLabel, 1);
			}
			return ;
		}
	}
	else if (graph[x][y] == 3 || graph[x][y] == 6){//如果下一个是蛇头
		if (life && lifeFlag && direction != 0)
			life--;
		this->removeChild(lifeTag, true);
		char life_str[30];
		sprintf(life_str, "Life: %d", life);
		lifeTag = CCLabelTTF::create(life_str, "font09.fnt", 15);
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		lifeTag->setPosition(ccp(size.width-150, 140));
		this->addChild(lifeTag, 1);
		if(direction != 0)
			dTemp = direction;
		direction = 0;
		if (!life)
		{
								for (int i = 0; i < 20; i++)//根据规则显示
						{
							for (int j = 0; j < 15; j++)
							{
								if (graph[i][j] == 1)
								sprite[i][j] = CCSprite::create("wall.png");
								else if (graph[i][j] == 2)
								sprite[i][j] = CCSprite::create("tail.png");
								else if (graph[i][j] == 3)
								sprite[i][j] = CCSprite::create("head.jpg");
								else
								sprite[i][j] = CCSprite::create("grid.jpg");
								sprite[i][j]->setPosition(ccp(32 * i + 16, 32 * j + 16));
								this->addChild(sprite[i][j], 1);
							}
						}
			flag = false;
			CCLabelTTF* pLabel = CCLabelTTF::create("Game Over!", "font09.fnt", 24);

			// Get window size and place the label upper. 
			CCSize size = CCDirector::sharedDirector()->getWinSize();
			pLabel->setPosition(ccp(size.width/2, size.height/2));

			// Add the label to HelloWorld layer as a child layer.
			this->addChild(pLabel, 1);
		}
			return ;
	}
	direction = dInput;//如果下一个为grid。或者bean

	switch (direction)//确定方向后，计算x，y
	{
	//向上
	case 1: x = Snack[0][head], y = Snack[1][head] + 1, lifeFlag = true;
		break;
	//向下
	case 2: x = Snack[0][head], y = Snack[1][head] - 1, lifeFlag = true;
		break;
	//向右
	case 3: x = Snack[0][head] + 1, y = Snack[1][head], lifeFlag = true;
		break;
	//向左
	case 4: x = Snack[0][head] - 1, y = Snack[1][head], lifeFlag = true;
		break;
	default: x = Snack[0][head], y = Snack[1][head], lifeFlag = false;
		break;
}
	
	 if (x == beanX && y == beanY)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Eat.mp3");
		length++;
		
		if (length%5 ==0)
		{

			length -= 8;
			level++;
			if (gameSpeed >70)
				gameSpeed -= 60;
			this->schedule(schedule_selector(GreedySnack::step), gameSpeed / 1000);
			this->removeChild(levelTag, true);
			char level_str[30];
			sprintf(level_str, "Level: %d", level);
			levelTag = CCLabelTTF::create(level_str, "font09.fnt", 15);
			CCSize size = CCDirector::sharedDirector()->getWinSize();
			levelTag->setPosition(ccp(size.width-80, 160));
			this->addChild(levelTag, 1);
			this->removeChild(speedTag, true);
			char speed_str[30];
			speed = 10 / (gameSpeed / 1000);
			sprintf(speed_str, "Speed: %d", speed);
			speedTag = CCLabelTTF::create(speed_str, "font09.fnt", 15);
//			CCSize size = CCDirector::sharedDirector()->getWinSize();
			speedTag->setPosition(ccp(size.width-70, 180));
			this->addChild(speedTag, 1);
		}
		graph[beanX][beanY] = 0;
		graph[Snack[0][head]][Snack[1][head]] = 2;
		head = (head + 1) % 100;
		graph[x][y] = 3;
		Snack[0][head] = x;
		Snack[1][head] = y;
		srand(time(0));
		do
		{
		beanX = rand() % 8 + 1;
		beanY = rand() % 8 + 1;
		} while (graph[beanX][beanY]);
		graph[beanX][beanY] = 2;
	 }
	else if(graph[x][y] == 1 )//如果下一步是墙wall//由于我缺少一个判断语句，所以在冲向蛇的时候，狂点后，可以把ai蛇吃掉！
	{
		if (life && lifeFlag && direction != 0)
			life--;
		this->removeChild(lifeTag, true);
		char life_str[30];
		sprintf(life_str, "Life: %d", life);
		lifeTag = CCLabelTTF::create(life_str, "font09.fnt", 15);
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		lifeTag->setPosition(ccp(size.width-150, 140));
		this->addChild(lifeTag, 1);
		if(direction != 0)
			dTemp = direction;
		direction = 0;
		if (!life)
		{
							for (int i = 0; i < 20; i++)//根据规则显示
					{
						for (int j = 0; j < 15; j++)
						{
							if (graph[i][j] == 1)
							sprite[i][j] = CCSprite::create("wall.png");
							else if (graph[i][j] == 2)
							sprite[i][j] = CCSprite::create("tail.png");
							else if (graph[i][j] == 3)
							sprite[i][j] = CCSprite::create("head.jpg");
							else
							sprite[i][j] = CCSprite::create("grid.jpg");
							sprite[i][j]->setPosition(ccp(32 * i + 16, 32 * j + 16));
							this->addChild(sprite[i][j], 1);
						}
					}

			if(flag)
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("GameOver.mp3");
			flag = false;
			CCLabelTTF* pLabel = CCLabelTTF::create("Game Over!", "font09.fnt", 24);

			// Get window size and place the label upper. 
			CCSize size = CCDirector::sharedDirector()->getWinSize();
			pLabel->setPosition(ccp(size.width/2, size.height/2));

			// Add the label to HelloWorld layer as a child layer.
			this->addChild(pLabel, 1);
		}
		return ;
	}
	else
	{
		graph[Snack[0][tail]][Snack[1][tail]] = 0;
		tail = (tail + 1) % 100;
		graph[Snack[0][head]][Snack[1][head]] = 2;
		head = (head + 1) % 100;
		graph[x][y] = 3;
		Snack[0][head] = x;
		Snack[1][head] = y;
	}

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 15; j++)
		this->removeChild(sprite[i][j], true);
	}
	for (int i = 0; i < 20; i++)//根据规则显示
	{
		for (int j = 0; j < 15; j++)
		{
			if (graph[i][j] == 1)
			sprite[i][j] = CCSprite::create("wall.png");
			else if (graph[i][j] == 2)
			sprite[i][j] = CCSprite::create("tail.png");
			else if (graph[i][j] == 3)
			sprite[i][j] = CCSprite::create("head.jpg");
			else
			sprite[i][j] = CCSprite::create("grid.jpg");
			sprite[i][j]->setPosition(ccp(32 * i + 16, 32 * j + 16));
			this->addChild(sprite[i][j], 1);
		}
	}
	
}

int GreedySnack::getbeanX()
{
	return beanX;
}
	
int GreedySnack::getbeanY(){
	return beanY;
}

int GreedySnack::getMusic()
{
	return music;
}

void GreedySnack::menuOnSetting(CCObject* pSender)
{
//	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("game.wma",music);
	//点击即进入主界面。helloworld
	CCScene* scene = NULL;
	do
	{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);
		Setting* pScene = Setting::create();
		CC_BREAK_IF(!pScene);
		scene->addChild(pScene);
		CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInB::create(0.5f, scene));
	} while (0);
}