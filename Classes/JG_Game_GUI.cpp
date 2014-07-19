#include "JG_Game_GUI.h"
#include <locale.h>

using namespace cocos2d;

JG_Game_GUI::JG_Game_GUI(void)
{

	
}

JG_Game_GUI::~JG_Game_GUI(void)
{

}


JG_Game_GUI * JG_Game_GUI::create(JG_Game_Main * game)
{
	JG_Game_GUI * GUI = new JG_Game_GUI();
	if (GUI && GUI->init(game))
	{
		GUI->autorelease();
		return GUI;
	}
	CC_SAFE_DELETE(GUI);
	return NULL;
}


bool JG_Game_GUI::init(JG_Game_Main* game)
{
	mainGame = game;

	gameMenu = CCMenu::create();
	this->addChild(gameMenu);
	gameMenu->setPosition(CCPointZero);

	gameMenu->retain();

	ResetInfos();
	
	screenSize = CCDirector::sharedDirector()->getWinSize();

	

	InitHUDItems();
	InitPauseMenuItems();
	InitEndRoundMenuItems();
	InitHighScoreMenuItems();

	HideGUIScreens();

	return true;
}
void JG_Game_GUI::ResetInfos()
{
	playerScore =0 ;
	playerReservedBallCount = 0;
	playerLifeCount = 0;
	playerRank = 0;
	highestScore = 0;
	highestScorePlayerName = "";


}

void JG_Game_GUI::InitHUDItems()
{
	scoreLabel = CreateLabel("0"
		, "fonts/font.fnt"
		, 0.3f
		, ccp(0.85, 0.7));

	reservedBallLabel = CreateLabel("0"
		, "fonts/font.fnt"
		, 0.3f
		, ccp(0.2, 0.8));

	ScoreGainAnimation = CCSequence::create(
		CCEaseInOut::create(CCScaleTo::create(0.3,2,2),0.5)
		,CCEaseInOut::create(CCScaleTo::create(0.3,1,1),0.5),NULL);
	ScoreGainAnimation->retain();

	ballAddButton = CreateButton("Cannon2.png", "Cannon2.png"
		, mainGame, menu_selector(JG_Game_Main::ReleaseBall)
		,CCPoint(0.1,0.8), "" );
}


void JG_Game_GUI::InitPauseMenuItems()
{		
	
	pauseButton = CreateButton("Buttons/Game/Pause_Normal.png"
		, "Buttons/Game/Pause_Selected.png" 
		, mainGame
		, menu_selector(JG_Game_Main::HandlePauseGame)
		, ccp(0.85, 0.9)
		, BUTTON_SOUND_EFFECT);

	resumeButton = CreateButton("Buttons/Game/Resume_Normal.png"
		, "Buttons/Game/Resume_Selected.png"
		, mainGame
		, menu_selector(JG_Game_Main::HandleResumeGame)
		, ccp(0.5, 0.8)
		, BUTTON_SOUND_EFFECT);

	resetButton = CreateButton("Buttons/Game/Reset_Normal.png"
		, "Buttons/Game/Reset_Selected.png"
		, mainGame
		, menu_selector(JG_Game_Main::HandleResetGame)
		, ccp(0.5, 0.55)
		, BUTTON_SOUND_EFFECT);

	exitToMainMenuButton = CreateButton("Buttons/Game/ExitToMainMenu_Normal.png"
		, "Buttons/Game/ExitToMainMenu_Normal.png"
		, mainGame
		, menu_selector(JG_Game_Main::HandleExitToMainMenu)
		, ccp(0.5, 0.3)
		, BUTTON_SOUND_EFFECT);

}

void JG_Game_GUI::InitEndRoundMenuItems()
{
	highestScoreLabel = CreateLabel(""
		, "fonts/font2.fnt"
		, 0.8
		, ccp(0.5, 0.9)
		, ccWHITE
		, 0.6);

	playerFinalScoreLabel = CreateLabel(" "
		, "fonts/font2.fnt"
		, 0.8
		, ccp(0.5, 0.7)
		, ccWHITE
		, 0.6);

	endRound_RetryButton = CreateButton("Buttons/Game/Reset_Normal.png"
		, "Buttons/Game/Reset_Selected.png"
		, mainGame
		, menu_selector(JG_Game_Main::HandleEndRoundScreenResetGame)
		, ccp(0.75, 0.2)
		, BUTTON_SOUND_EFFECT);

	endRound_ExitToMenuButton = CreateButton("Buttons/Game/ExitToMainMenu_Normal.png"
		, "Buttons/Game/ExitToMainMenu_Selected.png"
		, mainGame
		, menu_selector(JG_Game_Main::HandleEndRoundScreenExitToMainMenu)
		, ccp( 0.15, 0.2)
		, BUTTON_SOUND_EFFECT);

}

void JG_Game_GUI::InitHighScoreMenuItems()
{
	playerRankLabel = CreateLabel(""
		, "fonts/font2.fnt"
		, 0.8
		, ccp(0.4, 0.5)
		, ccWHITE
		, 0.6);
	

	playerNameTextBox = CCTextFieldTTF::textFieldWithPlaceHolder("", "fonts/font2.fnt", screenSize.height * 0.05f);
	
	playerNameTextBox->setString("Player");
	
	
	playerNameTextBox->setColor(ccYELLOW);
	playerNameTextBox->setPosition(ccp(screenSize.width*0.5,screenSize.height*0.5));
	
	this->addChild(playerNameTextBox,100);
}

void JG_Game_GUI::SetHUDVisibility(bool bVisible)
{
	scoreLabel->setVisible(bVisible);
	reservedBallLabel->setVisible(bVisible);
	ballAddButton->setVisible(bVisible);

}
void JG_Game_GUI::SetPauseScreenVisibility(bool bVisible)
{
	//TODO: wtf is this shit here ?!!pauseButton->setVisible(!bVisible);
	pauseButton->setVisible(!bVisible);
	resumeButton->setVisible(bVisible);
	resetButton->setVisible(bVisible);
	exitToMainMenuButton->setVisible(bVisible);
}

void JG_Game_GUI::SetEndRoundScreenVisibility(bool bVisible)
{
	//TODO: wtf is this shit here ?!!pauseButton->setVisible(!bVisible);
	pauseButton->setVisible(!bVisible);
	endRound_ExitToMenuButton->setVisible(bVisible);
	endRound_RetryButton->setVisible(bVisible);	
	playerFinalScoreLabel->setVisible(bVisible);
	highestScoreLabel->setVisible(bVisible);
}

void JG_Game_GUI::SetHighScoreScreenVisibility(bool bVisible)
{
	playerRankLabel->setVisible(bVisible);
	playerNameTextBox->setVisible(bVisible);
	/*if(bVisible)
		playerNameTextBox->attachWithIME();
	else
		playerNameTextBox->detachWithIME();*/
	

}

void JG_Game_GUI::HideGUIScreens()
{
	SetPauseScreenVisibility(false);
	SetEndRoundScreenVisibility(false);
	SetHighScoreScreenVisibility(false);
}

void JG_Game_GUI::SetEndRoundScreenInfos(int _playerScore,int _highestScore, CCString _highestScorePlayerName)
{
	playerScore = _playerScore;
	highestScore = _highestScore;
	highestScorePlayerName = _highestScorePlayerName;
	UpdateEndRoundScreen();
}

void JG_Game_GUI::UpdateEndRoundScreen()
{
	playerFinalScoreLabel->setString(CCString::createWithFormat("Score: %i", playerScore)->getCString());
	highestScoreLabel->setString(CCString::createWithFormat("HighScore: %s %i", highestScorePlayerName.getCString(), highestScore)->getCString());
}


void JG_Game_GUI::SetHighScoreScreenInfos(int _playerRank)
{
	playerRank = _playerRank;
	UpdateHighScoreScreen();

}

void JG_Game_GUI::UpdateHighScoreScreen()
{
	playerRankLabel->setString(CCString::createWithFormat("%i : ", playerRank)->getCString());
}

void JG_Game_GUI::SetPlayerScore(int score)
{
	playerScore = score;
	UpdatePlayerScore();
}

void JG_Game_GUI::UpdatePlayerScore()
{
	scoreLabel->setString(CCString::createWithFormat("%i", playerScore)->getCString());
	scoreLabel->runAction((CCAction *)ScoreGainAnimation->copy());
}

void JG_Game_GUI::SetPlayerReservedBall(int count)
{
	playerReservedBallCount = count;
	UpdatePlayerReservedBall();
}

void JG_Game_GUI::UpdatePlayerReservedBall()
{
	reservedBallLabel->setString(CCString::createWithFormat("%i", playerReservedBallCount)->getCString());
	reservedBallLabel->runAction((CCAction *)ScoreGainAnimation->copy());

}



bool JG_Game_GUI::IsPlayerNameTextBoxVisible()
{
	return playerNameTextBox->isVisible();
}

void JG_Game_GUI::CheckPlayerNameTextBoxTouched(CCTouch* touch)
{
	if(playerNameTextBox->boundingBox().containsPoint(touch->getLocation()))
		playerNameTextBox->attachWithIME();
		
}

std::string JG_Game_GUI::GetPlayerName()
{
	std::string temp =  playerNameTextBox->getString();
	return playerNameTextBox->getString();
}


JG_Button* JG_Game_GUI::CreateButton(CCString normalImage,CCString selectedImage, CCObject* target, SEL_MenuHandler selector, CCPoint positionRatio, CCString soundEffect)
{
	JG_Button * button;
	button = JG_Button::Create(CCSprite::create(normalImage.getCString())
		,CCSprite::create(selectedImage.getCString())
		,target
		,selector
		,soundEffect);
	button->setPosition(ccp(screenSize.width * positionRatio.x ,screenSize.height * positionRatio.y));

	gameMenu->addChild(button);
	return button;
}

CCLabelBMFont* JG_Game_GUI::CreateLabel(const char* text,const char* font, float widthRatio, CCPoint positionRatio, const ccColor3B& color, float scale)
{
	CCLabelBMFont* label  = CCLabelBMFont::create (text, font, screenSize.width * widthRatio);
	label->setColor(color);
	label->setScale(scale);

	label->setPosition(ccp(screenSize.width * positionRatio.x ,screenSize.height * positionRatio.y));

	label->setAnchorPoint(ccp(0.5,0.5));
	label->setAlignment(kCCTextAlignmentCenter);
	this->addChild(label);

	return label;
}
