#pragma once
#include "cocos2d.h"
#include "base_nodes\CCNode.h"
#include "textures\CCTexture2D.h"
#include "textures\CCTextureCache.h"
#include "label_nodes\CCLabelBMFont.h"


using namespace cocos2d;
class JG_Menu_GUI :
	public CCNode
{
	CCMenuItemSprite* StartGameButton;
	CCMenuItemSprite* ExitGameButton;
	CCMenuItemSprite* ShowOptionButton;
	CCMenuItemSprite* ShowHighScoresButton;
	CCMenuItemSprite* ReturnToMainMenuButton;

	SEL_MenuHandler startGameCallBack;
	SEL_MenuHandler exitCallBack;
	SEL_MenuHandler optionsCallBack;
	SEL_MenuHandler showHighScoresCallBack;

	CCObject* callBackTarget;
	  

	CCSize screenSize;

	std::vector<CCLabelBMFont*>* leaderBoardLabels;


	//TODO: find a better name
	CCMenu* ccMenu;
	
	void InitMenuGUI();
	void CreateCCMenu();

	

	void SetLeaderBoardHeaderVisibility(bool bVisible);
	void SetLeaderBoardHighScoresVisibility(bool bVisible);
	void ReturnToMainMenu(CCObject* pSender);

	void SetHighScoreLabelPosition(CCLabelBMFont* label);

	
	CCMenuItemSprite* CreateButton(CCString normalImage,CCString selectedImage, CCObject* target, SEL_MenuHandler selector, CCPoint positionRatio);

public:
	JG_Menu_GUI(void);
	virtual ~JG_Menu_GUI(void);

	static JG_Menu_GUI* CreateMenuGUI();

	void SetLeaderBoardScreenVisibility(bool bVisible);
	void SetMainMenuScreenVisibility(bool bVisible);
	void InsertHighScore(int rank,std::string playerName, int score);

	void SetCallBackTarget(CCObject* target);
	void SetStartGameCallBack( SEL_MenuHandler selector);
	void SetOptionCallBack( SEL_MenuHandler selector);
	void SetShowHighScoresCallBack(SEL_MenuHandler selector);
	void SetExitCallBack(SEL_MenuHandler selector);
	void CreateMenuButtons();
	void HideGUIScreens();
};

