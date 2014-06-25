#include "JG_Menu_Main.h"
#include "JG_Tutorial_Handler.h"

USING_NS_CC;

CCScene* JG_Menu_Main::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    JG_Menu_Main *layer = JG_Menu_Main::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool JG_Menu_Main::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    InitMainMenu();
    return true;
}

void JG_Menu_Main::InitMainMenu()
{
	bIsLeaderBoardCached = false;
	background = "MenuBackground.png";
	screenSize = CCDirector::sharedDirector()->getWinSize();
	InitMenuGUI();
	
	InitBackground();
	this->addChild(menuGUI,100);

}

void JG_Menu_Main::InitMenuGUI()
{
	menuGUI = JG_Menu_GUI::CreateMenuGUI();
	menuGUI->SetCallBackTarget(this);
	menuGUI->SetStartGameCallBack(menu_selector(JG_Menu_Main::StartGame));
	menuGUI->SetExitCallBack(menu_selector(JG_Menu_Main::ExitGame));
	menuGUI->SetShowHighScoresCallBack(menu_selector(JG_Menu_Main::ShowHighScores));
	menuGUI->SetOptionCallBack(menu_selector(JG_Menu_Main::ShowOption));
	menuGUI->SetPlayTutorialCallBack(menu_selector(JG_Menu_Main::PlayTutorial));

	menuGUI->CreateMenuButtons();
	menuGUI->HideGUIScreens();
	menuGUI->SetMainMenuScreenVisibility(true);
}
void JG_Menu_Main::InitBackground()
{	
	CCSprite* backgroundSprit = CCSprite::create(background.getCString());

	ScaleSpriteToDesignSize(backgroundSprit);

	backgroundSprit->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	this->addChild(backgroundSprit,-200);
}

void JG_Menu_Main::StartGame(CCObject* pSender)
{
	if(JG_Tutorial_Handler::mustPlayTutorial())
	{
		CCLOG("----------- Menu: paly-tutorial -------------");
		CCDirector::sharedDirector()->replaceScene(JG_Tutorial_Handler::scene(true));
	}
	else
	{
		CCLOG("----------- Menu: StartGame -------------");
		CCDirector::sharedDirector()->replaceScene(JG_Game_Main::scene());
	}

}

void JG_Menu_Main::PlayTutorial(CCObject* pSender)
{
	CCLOG("----------- Menu: paly-tutorial -------------");
	CCDirector::sharedDirector()->replaceScene(JG_Tutorial_Handler::scene(false));
}

void JG_Menu_Main::ShowOption(CCObject* pSender)
{
	CCLOG("----------- Menu: ShowOption -------------");

}

void JG_Menu_Main::ShowHighScores(CCObject* pSender)
{
	if(!bIsLeaderBoardCached)
	{
		JG_Score_Handler scoreHandler;
		vector<ScoreTableRecord>* table = scoreHandler.GetHighScoreTable();
		for(int i = 0 ; i<table->size(); i++)
			menuGUI->InsertHighScore(table->at(i).rank,table->at(i).name,table->at(i).score);
		bIsLeaderBoardCached = true;
	}
	menuGUI->HideGUIScreens();
	menuGUI->SetLeaderBoardScreenVisibility(true);
}

void JG_Menu_Main::ExitGame(CCObject* pSender)
{
	CCLOG("----------- Menu: ExitGame -------------");
	menuCloseCallback(pSender);
}

void JG_Menu_Main::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void JG_Menu_Main::menuPauseCallBack(CCObject* pSender)
{
	if(!CCDirector::sharedDirector()->isPaused())
		CCDirector::sharedDirector()->pause();
	else
		CCDirector::sharedDirector()->resume();
}

