#ifndef __MainMenu_SCENE_H__
#define __MainMenu_SCENE_H__

#include "cocos2d.h"
#include "JG_Menu_GUI.h"
#include "JG_Game_Main.h"
#include "JG_Score_Handler.h"
#include "Defines.h"

class JG_Game_Main;
class JG_Menu_GUI;

using namespace cocos2d;
class JG_Menu_Main : public cocos2d::CCLayer
{
	bool bIsLeaderBoardCached;
	JG_Menu_GUI* menuGUI;

	void InitMainMenu();
	void InitMenuGUI();

	CCString background;

	void InitBackground();


public:

	CCSize screenSize ;

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

	

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

	void StartGame(CCObject* pSender);
	void ShowOption(CCObject* pSender);
	void ShowHighScores(CCObject* pSender);
	void ExitGame(CCObject* pSender);
	void PlayTutorial(CCObject* psender);
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

	void menuPauseCallBack(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(JG_Menu_Main);
};

#endif // __MainMenu_SCENE_H__
