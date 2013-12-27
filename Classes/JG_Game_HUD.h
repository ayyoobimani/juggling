#pragma once
#include "cocos2d.h"
#include "JG_Game_Main.h"
#include "base_nodes\CCNode.h"
#include "textures\CCTexture2D.h"
#include "textures\CCTextureCache.h"
#include "label_nodes\CCLabelBMFont.h"

using namespace cocos2d;

class JG_Game_Main;


class JG_Game_HUD :
	public CCNode
{
	JG_Game_Main * mainGame;
public:
	
	CCSprite* gameResetSprite_On;
	CCSprite* gameResetSprite_Off;

	CCMenuItemSprite* pauseButton;
	CCMenuItemSprite* resetButton;
	CCMenuItemSprite* resumeButton;
	CCMenuItemSprite* exitGameButton;
	CCMenuItemSprite* exitToMainMenuButton;

	CCMenuItemSprite* ballAddButton;

	CCMenu* gameMenu;
	
	JG_Game_HUD(void);
	~JG_Game_HUD(void);
	static JG_Game_HUD* create(JG_Game_Main* game);
	bool init(JG_Game_Main* game);

	void Init_PauseMenu();

	
	CCPoint lifeDrawPosition;
	int lifeDrawPacing;
	CCTexture2D* lifeTexture_Active;
	CCTexture2D* lifeTexture_Diactive;
		

	CCLabelBMFont* scoreLabel;
	CCLabelBMFont* reservedBallLabel;

	CCLabelTTF* debugLabel;
	CCLabelTTF* balldepict;
	CCLabelTTF* handdepict;
	CCLabelTTF* prevballcount;
	CCFiniteTimeAction* ScoreGainAnimation;
	
	void ShowPauseScreen(bool bShow);
	void ShowEndRoundScreen(bool bShow);
	
	void draw();
	void DrawLife();


	void UpdateReservedBall();
	void UpdateScore();
};

