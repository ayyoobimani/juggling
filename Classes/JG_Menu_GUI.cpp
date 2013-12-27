#include "JG_Menu_GUI.h"


JG_Menu_GUI::JG_Menu_GUI(void)
{
}


JG_Menu_GUI::~JG_Menu_GUI(void)
{
}

JG_Menu_GUI* JG_Menu_GUI::CreateMenuGUI(JG_Menu_Main* menu)
{
	JG_Menu_GUI * GUI = new JG_Menu_GUI();
	if (GUI)
	{
		GUI->autorelease();
		GUI->InitMenuGUI(menu);
		return GUI;
	}
	CC_SAFE_DELETE(GUI);
	return NULL;
}

void JG_Menu_GUI::InitMenuGUI(JG_Menu_Main* menu)
{
	mainMenu =menu ;
	CreateCCMenu();
	CreateMenuButtons(menu);
}


void JG_Menu_GUI::CreateCCMenu()
{
	ccMenu = CCMenu::create();
	this->addChild(ccMenu);
	ccMenu->setPosition(CCPointZero);
	ccMenu->retain();

}
void JG_Menu_GUI::CreateMenuButtons(JG_Menu_Main* menu)
{
	StartGameButton = CreateButton("Buttons/Menu/Button_StartGame_Normal.png"
		, "Buttons/Menu/Button_StartGame_Selected.png"
		, mainMenu
		, menu_selector(JG_Menu_Main::StartGame)
		, ccp(0.5,0.8));

	ShowHighScoresButton = CreateButton("Buttons/Menu/Button_HighScores_Normal.png"
		, "Buttons/Menu/Button_HighScores_Selected.png"
		, mainMenu
		, menu_selector(JG_Menu_Main::ShowHighScores)
		, ccp(0.5,0.6));

	ShowOptionButton = CreateButton("Buttons/Menu/Button_Option_Normal.png"
		, "Buttons/Menu/Button_Option_Selected.png"
		, mainMenu
		, menu_selector(JG_Menu_Main::ShowOption)
		, ccp(0.5,0.4));

	ExitGameButton = CreateButton("Buttons/Menu/Button_ExitGame_Normal.png"
		, "Buttons/Menu/Button_ExitGame_Selected.png"
		, mainMenu
		, menu_selector(JG_Menu_Main::ExitGame)
		, ccp(0.5,0.2));
}


CCMenuItemSprite* JG_Menu_GUI::CreateButton(CCString normalImage,CCString selectedImage, CCObject* target, SEL_MenuHandler selector, CCPoint positionRatio)
{
	CCMenuItemSprite * menuItem;
	menuItem = CCMenuItemSprite::create(CCSprite::create(normalImage.getCString())
		,CCSprite::create(selectedImage.getCString())
		,target
		,selector);
	menuItem->setPosition(ccp(mainMenu->screenSize.width * positionRatio.x ,mainMenu->screenSize.height * positionRatio.y));

	ccMenu->addChild(menuItem);
	return menuItem;
}
