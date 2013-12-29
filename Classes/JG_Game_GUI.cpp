#include "JG_Game_GUI.h"


using namespace cocos2d;

JG_Game_GUI::JG_Game_GUI(void)
{

	mainGame = NULL;
}

JG_Game_GUI::~JG_Game_GUI(void)
{

}


JG_Game_GUI * JG_Game_GUI::create(JG_Game_Main* game)
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
	mainGame=game;

	gameMenu = CCMenu::create();
	this->addChild(gameMenu);
	gameMenu->setPosition(CCPointZero);

	gameMenu->retain();

	InitHUDItems();
	InitPauseMenuItems();
	InitEndRoundMenuItems();
	InitHighScoreMenuItems();

	HideGUIScreens();

	return true;
}

void JG_Game_GUI::InitHUDItems()
{
	lifeTexture_Active =CCTextureCache::sharedTextureCache()->addImage("heart_active.png");
	lifeTexture_Active->retain();

	lifeTexture_Diactive = CCTextureCache::sharedTextureCache()->addImage("heart_deactive.png");
	lifeTexture_Diactive->retain();

	lifeDrawPosition = ccp(mainGame->screenSize.width * 0.05
		,mainGame->screenSize.height-(lifeTexture_Active->getContentSizeInPixels().height * 1.1 ));

	lifeDrawPacing = lifeTexture_Active->getContentSizeInPixels().width + 2;

	scoreLabel =CCLabelBMFont::create ("0", "fonts/font.fnt", mainGame->screenSize.height * 0.3f);
	scoreLabel->setPosition(ccp(mainGame->screenSize.width * 0.85 ,mainGame->screenSize.height * 0.80) );
	this->addChild(scoreLabel);

	reservedBallLabel =CCLabelBMFont::create ("0", "fonts/font.fnt", mainGame->screenSize.height * 0.3f);
	reservedBallLabel->setPosition(ccp(mainGame->screenSize.width * 0.2 ,mainGame->screenSize.height * 0.80));
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
	ballAddButton->setPosition(ccp(mainGame->screenSize.width * 0.1 ,mainGame->screenSize.height * 0.8));
	gameMenu->addChild(ballAddButton);

}


void JG_Game_GUI::InitPauseMenuItems()
{		
	

	pauseButton = CCMenuItemSprite::create(CCSprite::create("Buttons/Game/Pause_Normal.png"),CCSprite::create("Buttons/Game/Pause_Selected.png")
		 ,mainGame
		,menu_selector(JG_Game_Main::PauseGame));
	pauseButton->retain();
	pauseButton->setPosition(ccp(mainGame->screenSize.width * 0.85 ,mainGame->screenSize.height * 0.90) );

	resumeButton = CCMenuItemSprite::create(CCSprite::create("Buttons/Game/Resume_Normal.png"),CCSprite::create("Buttons/Game/Resume_Selected.png")
		 ,mainGame
		 ,menu_selector(JG_Game_Main::ResumeGame));
	resumeButton->retain();
	resumeButton->setPosition(ccp(mainGame->screenSize.width * 0.5 ,mainGame->screenSize.height * 0.8));

	resetButton = CCMenuItemSprite::create(CCSprite::create("Buttons/Game/Reset_Normal.png"),CCSprite::create("Buttons/Game/Reset_Selected.png")
		 ,mainGame
		 ,menu_selector(JG_Game_Main::ResetGame));
	resetButton->retain();
	resetButton->setPosition(ccp(mainGame->screenSize.width * 0.5 ,mainGame->screenSize.height * 0.6));

	exitToMainMenuButton = CCMenuItemSprite::create(CCSprite::create("Buttons/Game/ExitToMainMenu_Normal.png"),CCSprite::create("Buttons/Game/ExitToMainMenu_Normal.png")
		 ,mainGame
		 ,menu_selector(JG_Game_Main::ExitToMainMenu));
	exitToMainMenuButton->retain();
	exitToMainMenuButton->setPosition(ccp(mainGame->screenSize.width * 0.5 ,mainGame->screenSize.height * 0.4));

	exitGameButton = CCMenuItemSprite::create(CCSprite::create("Buttons/Game/ExitGame_Normal.png"),CCSprite::create("Buttons/Game/ExitGame_Selected.png")
		 ,mainGame
		 ,menu_selector(JG_Game_Main::ExitGame));
	exitGameButton->retain();
	exitGameButton->setPosition(ccp(mainGame->screenSize.width * 0.5 ,mainGame->screenSize.height * 0.2));

	

	
	
	gameMenu->addChild(pauseButton);
	gameMenu->addChild(resumeButton);
	gameMenu->addChild(resetButton);
	gameMenu->addChild(exitGameButton);
	gameMenu->addChild(exitToMainMenuButton);
	
	

}

void JG_Game_GUI::InitEndRoundMenuItems()
{
	highestScoreLabel =CCLabelBMFont::create ("High Score: ", "fonts/font.fnt", mainGame->screenSize.height * 0.3f);
	highestScoreLabel->setPosition(ccp(mainGame->screenSize.width * 0.5 ,mainGame->screenSize.height * 0.80) );
	this->addChild(highestScoreLabel);

	endRoundScoreLabel = CCLabelBMFont::create ("High Score: ", "fonts/font.fnt", mainGame->screenSize.height * 0.3f);
	endRoundScoreLabel->setPosition(ccp(mainGame->screenSize.width * 0.5 ,mainGame->screenSize.height * 0.80) );
	this->addChild(endRoundScoreLabel);

	
}

void JG_Game_GUI::InitHighScoreMenuItems()
{
	playerRankLabel  = CCLabelBMFont::create ("High Score: ", "fonts/font.fnt", mainGame->screenSize.height * 0.3f);
	playerRankLabel->setPosition(ccp(mainGame->screenSize.width * 0.5 ,mainGame->screenSize.height * 0.80) );
	this->addChild(playerRankLabel);

	playerNameTextBox = CCTextFieldTTF::textFieldWithPlaceHolder("Enter Your Name", "", 25);
	playerNameTextBox->setPosition(ccp(mainGame->screenSize.width*0.5,mainGame->screenSize.height*0.5));
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

	pauseButton->setVisible(!bVisible);
	resumeButton->setVisible(bVisible);
	resetButton->setVisible(bVisible);
	exitToMainMenuButton->setVisible(bVisible);
	exitGameButton->setVisible(bVisible);	
}

void JG_Game_GUI::SetEndRoundScreenVisibility(bool bVisible)
{

	resetButton->setVisible(bVisible);
	exitToMainMenuButton->setVisible(bVisible);
	exitGameButton->setVisible(bVisible);	
	pauseButton->setVisible(!bVisible);
}

void JG_Game_GUI::SetHighScoreScreenVisibility(bool bVisible)
{
	playerRankLabel->setVisible(bVisible);
	playerNameTextBox->setVisible(bVisible);
	if(bVisible)
		playerNameTextBox->attachWithIME();
	else
		playerNameTextBox->detachWithIME();
	

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
	for(i = 0 ; i< mainGame->lifeCount ; ++i)
	{
		lifeTexture_Active->drawAtPoint(lifeDrawPosition+ ccp(i*lifeDrawPacing,0));
	}
	for( i ; i<MAX_LIFE_COUNT; ++i)
	{
		lifeTexture_Diactive->drawAtPoint(lifeDrawPosition+ ccp(i*lifeDrawPacing,0));
	}
	
}



void JG_Game_GUI::UpdateScore()
{
	scoreLabel->setString(CCString::createWithFormat("%i", mainGame->score)->getCString());
	scoreLabel->runAction((CCAction *)ScoreGainAnimation->copy());
}

void JG_Game_GUI::UpdateReservedBall()
{
	reservedBallLabel->setString(CCString::createWithFormat("%i", mainGame->reservedBallCount)->getCString());
	reservedBallLabel->runAction((CCAction *)ScoreGainAnimation->copy());

}

