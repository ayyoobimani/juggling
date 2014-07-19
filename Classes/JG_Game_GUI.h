#pragma once
#include "cocos2d.h"
#include "JG_Game_Main.h"
#include "base_nodes\CCNode.h"
#include "textures\CCTexture2D.h"
#include "textures\CCTextureCache.h"
#include "label_nodes\CCLabelBMFont.h"
#include "text_input_node\CCTextFieldTTF.h"
#include <string>

#define BUTTON_SOUND_EFFECT "Sounds/ButtonSelected.mp3"


using namespace cocos2d;


class JG_Game_Main;


class JG_Game_GUI :
	public CCNode
{
	int playerScore;
	int playerReservedBallCount;
	int playerLifeCount;
	int playerRank;
	
	int highestScore;
	CCString highestScorePlayerName;

	CCSize screenSize;
	CCMenu* gameMenu;
	JG_Game_Main* mainGame;

	void UpdateEndRoundScreen();
	void UpdateHighScoreScreen();
	void UpdatePlayerScore();
	void UpdatePlayerReservedBall();


	JG_Button* CreateButton(CCString normalImage,CCString selectedImage, CCObject* target, SEL_MenuHandler selector, CCPoint positionRatio, CCString soundEffect);

	CCLabelBMFont* CreateLabel(const char* text, const char* font, float widthRatio, CCPoint positionRatio, const ccColor3B& color = ccWHITE , float scale = 1);

	
	

public:
		
	JG_Game_GUI(void);
	~JG_Game_GUI(void);
	
	//CCSprite* gameResetSprite_On;
	//CCSprite* gameResetSprite_Off;

	CCMenuItemSprite* pauseButton;
	CCMenuItemSprite* resetButton;
	CCMenuItemSprite* resumeButton;
	CCMenuItemSprite* exitToMainMenuButton;
	CCMenuItemSprite* ballAddButton;

	CCMenuItemSprite* endRound_RetryButton;
	CCMenuItemSprite* endRound_ExitToMenuButton;

	CCLabelBMFont* scoreLabel;

	CCLabelBMFont* reservedBallLabel;
	CCLabelBMFont* highestScoreLabel;

	CCLabelBMFont* playerFinalScoreLabel;
	CCLabelBMFont* playerRankLabel;
	CCTextFieldTTF* playerNameTextBox;



	/*************tutorial******************/
	CCMenuItemSprite* nextTutorialPageButton;
	void initialTutorialItems();
	

	CCFiniteTimeAction* ScoreGainAnimation;


	

	static JG_Game_GUI* create(JG_Game_Main* game);
	bool init(JG_Game_Main* game);

	void InitHUDItems();
	void InitPauseMenuItems();
	void InitEndRoundMenuItems();
	void InitHighScoreMenuItems();

	
	void SetPauseScreenVisibility(bool bVisible);
	void SetHighScoreScreenVisibility(bool bVisible);
	void SetEndRoundScreenVisibility(bool bVisible);
	void SetHUDVisibility(bool bVisible);
	
	void HideGUIScreens();
	
	void ResetInfos();
	
	
	//TODO: find a better categories for When Player reach a high score and when not
	void SetEndRoundScreenInfos(int _playerScore,int _highestScore, CCString _highestScorePlayerName);
	void SetHighScoreScreenInfos(int _playerRank);
	void SetPlayerScore(int score);
	void SetPlayerReservedBall(int ballCount);

	bool IsPlayerNameTextBoxVisible();
	void CheckPlayerNameTextBoxTouched(CCTouch* touch);

	std::string GetPlayerName();


	
};

