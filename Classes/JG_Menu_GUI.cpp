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
	CCString soundEffect = "Sounds/ButtonSelected.mp3";
	startGameButton = CreateButton("Buttons/Menu/Button_StartGame_Normal.png"
		, "Buttons/Menu/Button_StartGame_Selected.png"
		, callBackTarget
		, startGameCallBack
		, ccp(0.9,0.15)
		, soundEffect);

	showHighScoresButton = CreateButton("Buttons/Menu/Plain2.png"
		, "Buttons/Menu/Button_HighScores_Selected.png"
		, callBackTarget
		, showHighScoresCallBack
		, ccp(0.1,0.65)
		, soundEffect);

	showOptionButton = CreateButton("Buttons/Menu/Plain.png"
		, "Buttons/Menu/Button_Option_Selected.png"
		, callBackTarget
		, optionsCallBack
		, ccp(0.1,0.15)
		, soundEffect);

	returnToMainMenuButton = CreateButton("Buttons/Menu/Button_ExitGame_Normal.png"
		, "Buttons/Menu/Button_ExitGame_Selected.png"
		, this
		, menu_selector(JG_Menu_GUI::ReturnToMainMenu)
		, ccp(0.5,0.2)
		,soundEffect);

	howToPlayButton = CreateButton("Buttons/Menu/Button_How_To_Play_Normal.png"
		, "Buttons/Menu/Button_How_To_Play_Selected.png"
		, callBackTarget
		, playTutorialCallBack
		, ccp(0.1,0.40)
		, soundEffect);
}


JG_Button* JG_Menu_GUI::CreateButton(CCString normalImage,CCString selectedImage, CCObject* target, SEL_MenuHandler selector, CCPoint positionRatio, CCString soundEffect)
{
	JG_Button * button;
	button = JG_Button::Create(CCSprite::create(normalImage.getCString())
		,CCSprite::create(selectedImage.getCString())
		,target
		,selector
		,soundEffect);
	button->setPosition(ccp(screenSize.width * positionRatio.x ,screenSize.height * positionRatio.y));

	ccMenu->addChild(button);
	return button;
}

void JG_Menu_GUI::HideGUIScreens()
{
	SetLeaderBoardScreenVisibility(false);
	SetMainMenuScreenVisibility(false);
}

void JG_Menu_GUI::SetMainMenuScreenVisibility(bool bVisible)
{
	startGameButton->setVisible(bVisible);
	showOptionButton->setVisible(bVisible);
	showHighScoresButton->setVisible(bVisible);
	howToPlayButton->setVisible(bVisible);
}

void JG_Menu_GUI::SetLeaderBoardScreenVisibility(bool bVisible)
{
	SetLeaderBoardHighScoresVisibility(bVisible);

	returnToMainMenuButton->setVisible(bVisible);
}


void JG_Menu_GUI::SetLeaderBoardHighScoresVisibility(bool bVisible)
{
	for(int i = 0 ; i<leaderBoardLabels->size();i++)
		leaderBoardLabels->at(i)->setVisible(bVisible);
}

void JG_Menu_GUI::InsertHighScore(int rank,std::string playerName, int score)
{
	CCLabelBMFont* label = CCLabelBMFont::create ("0", "fonts/font2.fnt", screenSize.width * 0.8f);

	//label->setColor(ccBLUE);
	label->setScale(0.6);

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

