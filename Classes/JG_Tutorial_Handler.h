#pragma once

#include "cocos2d.h"
#include "JG_Game_Main.h"

#define NumberOfPicturs 2
using namespace cocos2d;

class JG_Tutorial_Handler: public cocos2d::CCLayer
{
	int currentShowingPictureNumber;
	bool  bIsPlayingForFirstTime;
	CCSprite* currentBackGround;
	bool bMustStartGameAtEnd;

	CCMenuItemSprite* backButton;
	CCMenuItemSprite* nextButton;
	CCMenuItemSprite* backToMenueButton;
	static CCString haveSeenTutorial;

	CCSize screenSize;
	CCMenu * ccMenue;

	void CreateCCMenu();
	CCMenuItemSprite* CreateButton(CCString normalImage,CCString selectedImage, CCObject* target, SEL_MenuHandler selector, CCPoint positionRatio);
	void createButtons();

	
public:
	
	JG_Tutorial_Handler(bool mustStartGame);
	~JG_Tutorial_Handler(void);

	void playTutorial();
	static CCScene* scene(bool mustStartGame);

	static bool mustPlayTutorial();
	void showNextPage(CCObject* pSender);
	void showPrevPage(CCObject* pSender);

	void toggleNextToPlayButton(bool btoggletoPlay);
	void goBackToMainMenue(CCObject* pSender);
	
};

