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


	static JG_Game_HUD* create(JG_Game_Main* game);

	CCPoint lifeDrawPosition;
	int lifeDrawPacing;

	CCTexture2D * lifeTexture;
	CCLabelBMFont * scoreFont;
	JG_Game_HUD(void);
	~JG_Game_HUD(void);

	bool init(JG_Game_Main* game);
	void draw();
	void DrawLife();
	void DrawScore();
};

