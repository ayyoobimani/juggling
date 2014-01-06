#include "JG_Game_GUI.h"


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

	debugLabel = CCLabelBMFont::create ("0", "fonts/font.fnt", screenSize.height * 0.3f);
	debugLabel->setPosition(ccp(screenSize.width * 0.5 ,screenSize.height * 0.10) );
	this->addChild(debugLabel);

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
	lifeTexture_Active =CCTextureCache::sharedTextureCache()->addImage("heart_active.png");
	lifeTexture_Active->retain();

	lifeTexture_Diactive = CCTextureCache::sharedTextureCache()->addImage("heart_deactive.png");
	lifeTexture_Diactive->retain();

	lifeDrawPosition = ccp(screenSize.width * 0.05
		,screenSize.height-(lifeTexture_Active->getContentSizeInPixels().height * 1.1 ));

	lifeDrawPacing = lifeTexture_Active->getContentSizeInPixels().width + 2;

	scoreLabel =CCLabelBMFont::create ("0", "fonts/font.fnt", screenSize.height * 0.3f);
	scoreLabel->setPosition(ccp(screenSize.width * 0.85 ,screenSize.height * 0.80) );
	this->addChild(scoreLabel);

	reservedBallLabel =CCLabelBMFont::create ("0", "fonts/font.fnt", screenSize.height * 0.3f);
	reservedBallLabel->setPosition(ccp(screenSize.width * 0.2 ,screenSize.height * 0.80));
	this->addChild(reservedBallLabel);

	ScoreGainAnimation = CCSequence::create(
		CCEaseInOut::create(CCScaleTo::create(0.3,2,2),0.5)
		,CCEaseInOut::create(CCScaleTo::create(0.3,1,1),0.5),NULL);
	ScoreGainAnimation->retain();

	ballAddButton = CCMenuItemSprite::create(CCSprite::create("BallAdder_Normal.png"),CCSprite::create("BallAdder_Selected.png")
		 ,mainGame
		 ,menu_selector(JG_Game_Main::ReleaseBall));
	ballAddButton->setOpacity(170);
	ballAddButton->retain();
	ballAddButton->setPosition(ccp(screenSize.width * 0.1 ,screenSize.height * 0.8));
	gameMenu->addChild(ballAddButton);

}


void JG_Game_GUI::InitPauseMenuItems()
{		
	

	pauseButton = CCMenuItemSprite::create(CCSprite::create("Buttons/Game/Pause_Normal.png"),CCSprite::create("Buttons/Game/Pause_Selected.png")
		 ,mainGame
		 ,menu_selector(JG_Game_Main::HandlePauseGame));
	pauseButton->retain();
	pauseButton->setPosition(ccp(screenSize.width * 0.85 ,screenSize.height * 0.90) );

	resumeButton = CCMenuItemSprite::create(CCSprite::create("Buttons/Game/Resume_Normal.png"),CCSprite::create("Buttons/Game/Resume_Selected.png")
		 ,mainGame
		 ,menu_selector(JG_Game_Main::HandleResumeGame));
	resumeButton->retain();
	resumeButton->setPosition(ccp(screenSize.width * 0.5 ,screenSize.height * 0.8));

	resetButton = CCMenuItemSprite::create(CCSprite::create("Buttons/Game/Reset_Normal.png"),CCSprite::create("Buttons/Game/Reset_Selected.png")
		 ,mainGame
		 ,menu_selector(JG_Game_Main::HandleResetGame));
	resetButton->retain();
	resetButton->setPosition(ccp(screenSize.width * 0.5 ,screenSize.height * 0.6));

	exitToMainMenuButton = CCMenuItemSprite::create(CCSprite::create("Buttons/Game/ExitToMainMenu_Normal.png"),CCSprite::create("Buttons/Game/ExitToMainMenu_Normal.png")
		 ,mainGame
		 ,menu_selector(JG_Game_Main::HandleExitToMainMenu));
	exitToMainMenuButton->retain();
	exitToMainMenuButton->setPosition(ccp(screenSize.width * 0.5 ,screenSize.height * 0.4));

	exitGameButton = CCMenuItemSprite::create(CCSprite::create("Buttons/Game/ExitGame_Normal.png"),CCSprite::create("Buttons/Game/ExitGame_Selected.png")
		 ,mainGame
		 ,menu_selector(JG_Game_Main::HandleExitGame));
	exitGameButton->retain();
	exitGameButton->setPosition(ccp(screenSize.width * 0.5 ,screenSize.height * 0.2));

	

	
	
	gameMenu->addChild(pauseButton);
	gameMenu->addChild(resumeButton);
	gameMenu->addChild(resetButton);
	gameMenu->addChild(exitGameButton);
	gameMenu->addChild(exitToMainMenuButton);
	
	

}

void JG_Game_GUI::InitEndRoundMenuItems()
{
	highestScoreLabel =CCLabelBMFont::create (" ", "fonts/arial16.fnt", screenSize.height * 0.3f);
	highestScoreLabel->setPosition(ccp(screenSize.width * 0.5 ,screenSize.height * 0.90) );
	highestScoreLabel->setAnchorPoint(ccp(0.5,0.5));
	highestScoreLabel->setAlignment(kCCTextAlignmentCenter);
	this->addChild(highestScoreLabel);

	playerFinalScoreLabel = CCLabelBMFont::create (" ", "fonts/arial16.fnt", screenSize.height * 0.3f);
	playerFinalScoreLabel->setPosition(ccp(screenSize.width * 0.5 ,screenSize.height * 0.70) );
	highestScoreLabel->setAnchorPoint(ccp(0.5,0.5));
	this->addChild(playerFinalScoreLabel);

	endRound_RetryButton = CCMenuItemSprite::create(CCSprite::create("Buttons/Game/Reset_Normal.png"),CCSprite::create("Buttons/Game/Reset_Selected.png")
		 ,mainGame
		 ,menu_selector(JG_Game_Main::HandleEndRoundScreenResetGame));
	endRound_RetryButton->retain();
	endRound_RetryButton->setPosition(ccp(screenSize.width * 0.75 ,screenSize.height * 0.2));
	gameMenu->addChild(endRound_RetryButton);

	endRound_ExitToMenuButton = CCMenuItemSprite::create(CCSprite::create("Buttons/Game/ExitToMainMenu_Normal.png"),CCSprite::create("Buttons/Game/ExitToMainMenu_Selected.png")
		 ,mainGame
		 ,menu_selector(JG_Game_Main::HandleEndRoundScreenResetGame));
	endRound_ExitToMenuButton->retain();
	endRound_ExitToMenuButton->setPosition(ccp(screenSize.width * 0.15 ,screenSize.height * 0.2));
	gameMenu->addChild(endRound_ExitToMenuButton);




}

void JG_Game_GUI::InitHighScoreMenuItems()
{
	playerRankLabel  = CCLabelBMFont::create ("", "fonts/arial16.fnt", screenSize.height * 0.3f);
	playerRankLabel->setPosition(ccp(screenSize.width * 0.4 ,screenSize.height * 0.5) );
	this->addChild(playerRankLabel);

	playerNameTextBox = CCTextFieldTTF::textFieldWithPlaceHolder("", "", screenSize.height * 0.05f);
	playerNameTextBox->setString("Player");
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
	//TODO: wtf is this shit heare ?!!pauseButton->setVisible(!bVisible);
	pauseButton->setVisible(!bVisible);
	resumeButton->setVisible(bVisible);
	resetButton->setVisible(bVisible);
	exitToMainMenuButton->setVisible(bVisible);
	exitGameButton->setVisible(bVisible);	
}

void JG_Game_GUI::SetEndRoundScreenVisibility(bool bVisible)
{
	//TODO: wtf is this shit heare ?!!pauseButton->setVisible(!bVisible);
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



void JG_Game_GUI::draw()
{
	if(mainGame!=NULL)
	{
		//DrawLife();
	}
}


void JG_Game_GUI::DrawLife()
{
	int i;
	for(i = 0 ; i< playerLifeCount ; ++i)
	{
		lifeTexture_Active->drawAtPoint(lifeDrawPosition+ ccp(i*lifeDrawPacing,0));
	}
	for( i ; i<MAX_LIFE_COUNT; ++i)
	{
		lifeTexture_Diactive->drawAtPoint(lifeDrawPosition+ ccp(i*lifeDrawPacing,0));
	}
	
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

void JG_Game_GUI::SetDebugLabelInfo(string debug)
{
	debugLabel->setString(debug.c_str());
	debugLabel->setVisible(true);
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