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
	JG_Game_HUD(void);
	~JG_Game_HUD(void);
	static JG_Game_HUD* create(JG_Game_Main* game);
	bool init(JG_Game_Main* game);

	
	CCPoint lifeDrawPosition;
	int lifeDrawPacing;
	CCTexture2D * lifeTexture_Active;
	CCTexture2D * lifeTexture_Diactive;
		

	CCLabelBMFont * scoreLabel;
	CCFiniteTimeAction* ScoreGainAnimation;
	

	
	void draw();
	void DrawLife();
	void UpdateScore();
};

