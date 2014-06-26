#include "JG_Menu_GUI.h"



JG_Menu_GUI::JG_Menu_GUI(void)
{
}


JG_Menu_GUI::~JG_Menu_GUI(void)
{
}

JG_Menu_GUI* JG_Menu_GUI::CreateMenuGUI()
{
	JG_Menu_GUI * GUI = new JG_Menu_GUI();
	if (GUI)
	{
		GUI->autorelease();
		GUI->InitMenuGUI();
		return GUI;
	}
	CC_SAFE_DELETE(GUI);
	return NULL;
}

void JG_Menu_GUI::InitMenuGUI()
{

	screenSize = CCDirector::sharedDirector()->getWinSize();
	leaderBoardLabels = new std::vector<CCLabelBMFont*>;

	CreateCCMenu();
	//CreateMenuButtons();
}


void JG_Menu_GUI::CreateCCMenu()
{
	ccMenu = CCMenu::create();
	this->addChild(ccMenu);
	ccMenu->setPosition(CCPointZero);
	ccMenu->retain();
}
void JG_Menu_GUI::CreateMenuButtons()
{

	StartGameButton = CreateButton("Buttons/Menu/Button_StartGame_Normal.png"
		, "Buttons/Menu/Button_StartGame_Selected.png"
		, callBackTarget
		, startGameCallBack
		, ccp(0.5,0.8));

	ShowHighScoresButton = CreateButton("Buttons/Menu/Button_HighScores_Normal.png"
		, "Buttons/Menu/Button_HighScores_Selected.png"
		, callBackTarget
		, showHighScoresCallBack
		, ccp(0.5,0.6));

	ShowOptionButton = CreateButton("Buttons/Menu/Button_Option_Normal.png"
		, "Buttons/Menu/Button_Option_Selected.png"
		, callBackTarget
		, optionsCallBack
		, ccp(0.5,0.4));

	ExitGameButton = CreateButton("Buttons/Menu/Button_ExitGame_Normal.png"
		, "Buttons/Menu/Button_ExitGame_Selected.png"
		, callBackTarget
		, exitCallBack
		, ccp(0.5,0.2));

	ReturnToMainMenuButton = CreateButton("Buttons/Menu/Button_ExitGame_Normal.png"
		, "Buttons/Menu/Button_ExitGame_Selected.png"
		, this
		, menu_selector(JG_Menu_GUI::ReturnToMainMenu)
		, ccp(0.5,0.2));

	howToPlayButton = CreateButton("Buttons/Menu/Button_How_To_Play_Normal.png"
		, "Buttons/Menu/Button_How_To_Play_Selected.png"
		, callBackTarget
		, playTutorialCallBack
		, ccp(0.2,0.1));
}


CCMenuItemSprite* JG_Menu_GUI::CreateButton(CCString normalImage,CCString selectedImage, CCObject* target, SEL_MenuHandler selector, CCPoint positionRatio)
{
	CCMenuItemSprite * menuItem;
	menuItem = CCMenuItemSprite::create(CCSprite::create(normalImage.getCString())
		,CCSprite::create(selectedImage.getCString())
		,target
		,selector);
	menuItem->setPosition(ccp(screenSize.width * positionRatio.x ,screenSize.height * positionRatio.y));

	ccMenu->addChild(menuItem);
	return menuItem;
}

void JG_Menu_GUI::HideGUIScreens()
{
	SetLeaderBoardScreenVisibility(false);
	SetMainMenuScreenVisibility(false);
}

void JG_Menu_GUI::SetMainMenuScreenVisibility(bool bVisible)
{
	StartGameButton->setVisible(bVisible);
	ShowOptionButton->setVisible(bVisible);
	ShowHighScoresButton->setVisible(bVisible);
	ExitGameButton->setVisible(bVisible);
	howToPlayButton->setVisible(bVisible);
}

void JG_Menu_GUI::SetLeaderBoardScreenVisibility(bool bVisible)
{
	SetLeaderBoardHeaderVisibility(bVisible);
	SetLeaderBoardHighScoresVisibility(bVisible);
}

void JG_Menu_GUI::SetLeaderBoardHeaderVisibility(bool bVisible)
{

}

void JG_Menu_GUI::SetLeaderBoardHighScoresVisibility(bool bVisible)
{
	for(int i = 0 ; i<leaderBoardLabels->size();i++)
		leaderBoardLabels->at(i)->setVisible(bVisible);
}

void JG_Menu_GUI::InsertHighScore(int rank,std::string playerName, int score)
{
	CCLabelBMFont* label = CCLabelBMFont::create ("0", "fonts/test.fnt", screenSize.width * 3.0f);
	
	SetHighScoreLabelPosition(label);

	label->setString(CCString::createWithFormat("%i: %s  %i", rank,playerName.c_str(),score)->getCString());

	leaderBoardLabels->push_back(label);
	this->addChild(label,200);
}

void JG_Menu_GUI::SetHighScoreLabelPosition(CCLabelBMFont* label)
{
	label->setAnchorPoint(ccp(0.5,0.5));
	if(leaderBoardLabels->size()==0)
		label->setPosition(screenSize.width*0.5,screenSize.height*0.8);
	else
	{
		label->setPosition(leaderBoardLabels->at(leaderBoardLabels->size()-1)->getPosition());
		label->setPositionY(label->getPositionY() - screenSize.height*0.1);
	}
}

void JG_Menu_GUI::SetCallBackTarget(CCObject* target)
{
	callBackTarget = target;
}

void JG_Menu_GUI::SetStartGameCallBack( SEL_MenuHandler selector)
{
	startGameCallBack = selector;
}

void JG_Menu_GUI::SetOptionCallBack( SEL_MenuHandler selector)
{
	optionsCallBack = selector;
}

void JG_Menu_GUI::SetShowHighScoresCallBack(SEL_MenuHandler selector)
{
	showHighScoresCallBack = selector;
}

void JG_Menu_GUI::SetExitCallBack(SEL_MenuHandler selector)
{
	exitCallBack = selector;
}

void JG_Menu_GUI::SetPlayTutorialCallBack(SEL_MenuHandler selector)
{
	playTutorialCallBack = selector;
}
										   

void JG_Menu_GUI::ReturnToMainMenu(CCObject* pSender)
{
	HideGUIScreens();
	SetMainMenuScreenVisibility(true);
}

