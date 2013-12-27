#include "JG_Menu_Main.h"

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
	screenSize = CCDirector::sharedDirector()->getWinSize();
	menuGUI = JG_Menu_GUI::CreateMenuGUI(this);
	this->addChild(menuGUI,100);

}

void JG_Menu_Main::StartGame(CCObject* pSender)
{
	CCLOG("----------- Menu: StartGame -------------");
	CCDirector::sharedDirector()->replaceScene(JG_Game_Main::scene());

}

void JG_Menu_Main::ShowOption(CCObject* pSender)
{
	CCLOG("----------- Menu: ShowOption -------------");

}

void JG_Menu_Main::ShowHighScores(CCObject* pSender)
{
	CCLOG("----------- Menu: ShowHighScores -------------");

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

