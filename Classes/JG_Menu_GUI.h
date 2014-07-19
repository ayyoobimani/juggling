#pragma once
#include "cocos2d.h"
#include "base_nodes/CCNode.h"
#include "textures/CCTexture2D.h"
#include "textures/CCTextureCache.h"
#include "label_nodes/CCLabelBMFont.h"
#include "JG_Button.h"

#define BUTTON_SOUND_EFFECT "Sounds/ButtonSelected.mp3"

using namespace cocos2d;
class JG_Menu_GUI :
	public CCNode
{
	JG_Button* startGameButton;
	JG_Button* showOptionButton;
	JG_Button* showHighScoresButton;
	JG_Button* returnToMainMenuButton;
	JG_Button* howToPlayButton;

	SEL_MenuHandler startGameCallBack;
	SEL_MenuHandler exitCallBack;
	SEL_MenuHandler optionsCallBack;
	SEL_MenuHandler showHighScoresCallBack;
	SEL_MenuHandler playTutorialCallBack;

	CCObject* callBackTarget;
	  

	CCSize screenSize;

	std::vector<CCLabelBMFont*>* leaderBoardLabels;


	//TODO: find a better name
	CCMenu* ccMenu;
	
	void InitMenuGUI();
	void CreateCCMenu();


	void SetLeaderBoardHighScoresVisibility(bool bVisible);
	void ReturnToMainMenu(CCObject* pSender);
	

	void SetHighScoreLabelPosition(CCLabelBMFont* label);

	
	JG_Button* CreateButton(CCString normalImage,CCString selectedImage, CCObject* target, SEL_MenuHandler selector, CCPoint positionRatio, CCString soundEffect);

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
	void SetPlayTutorialCallBack(SEL_MenuHandler selector);
	void CreateMenuButtons();
	void HideGUIScreens();
};

