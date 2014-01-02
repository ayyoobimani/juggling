#pragma once
#include "cocos2d.h"
#include "JG_Game_Main.h"
#include "base_nodes\CCNode.h"
#include "textures\CCTexture2D.h"
#include "textures\CCTextureCache.h"
#include "label_nodes\CCLabelBMFont.h"
#include "text_input_node\CCTextFieldTTF.h"

using namespace cocos2d;


class JG_Game_Main;


class JG_Game_GUI :
	public CCNode
{
	int playerScore;
	int playerReservedBallCount;
	int playerLifeCount;

	CCSize screenSize;
	CCMenu* gameMenu;
	JG_Game_Main* mainGame;

	void UpdatePlayerScore();
	void UpdatePlayerReservedBall();

public:
		
	JG_Game_GUI(void);
	~JG_Game_GUI(void);
	
	//CCSprite* gameResetSprite_On;
	//CCSprite* gameResetSprite_Off;

	CCMenuItemSprite* pauseButton;
	CCMenuItemSprite* resetButton;
	CCMenuItemSprite* resumeButton;
	CCMenuItemSprite* exitGameButton;
	CCMenuItemSprite* exitToMainMenuButton;
	CCMenuItemSprite* ballAddButton;

	CCLabelBMFont* scoreLabel;

	CCLabelBMFont* reservedBallLabel;
	CCLabelBMFont* highestScoreLabel;

	CCLabelBMFont* playerFinalScoreLabel;
	CCLabelBMFont* playerRankLabel;
	CCTextFieldTTF* playerNameTextBox;





	

		
	CCPoint lifeDrawPosition;
	int lifeDrawPacing;
	CCTexture2D* lifeTexture_Active;
	CCTexture2D* lifeTexture_Diactive;
		



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
	
	
	
	
	void draw();
	void DrawLife();


	void SetPlayerScore(int score);
	void SetPlayerReservedBall(int ballCount);
	
};

