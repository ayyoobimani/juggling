#pragma once
#include "base_nodes\CCNode.h"
#include "textures\CCTexture2D.h"
#include "textures\CCTextureCache.h"
#include "label_nodes\CCLabelBMFont.h"

#include "JG_Menu_Main.h"

class JG_Menu_Main;

using namespace cocos2d;
class JG_Menu_GUI :
	public CCNode
{
	CCMenuItemSprite* StartGameButton;
	CCMenuItemSprite* ExitGameButton;
	CCMenuItemSprite* ShowOptionButton;
	CCMenuItemSprite* ShowHighScoresButton;

	JG_Menu_Main* mainMenu;

	//TODO: find a better name
	CCMenu* ccMenu;

	void InitMenuGUI(JG_Menu_Main* menu);
	void CreateCCMenu();
	void CreateMenuButtons(JG_Menu_Main* menu);

	
	CCMenuItemSprite* CreateButton(CCString normalImage,CCString selectedImage, CCObject* target, SEL_MenuHandler selector, CCPoint positionRatio);

public:
	JG_Menu_GUI(void);
	virtual ~JG_Menu_GUI(void);

	static JG_Menu_GUI* CreateMenuGUI(JG_Menu_Main* menu);

	

	
};

