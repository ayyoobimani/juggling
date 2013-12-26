#include "JG_Game_HUD.h"


using namespace cocos2d;

JG_Game_HUD::JG_Game_HUD(void)
{

	mainGame = NULL;
}


JG_Game_HUD * JG_Game_HUD::create(JG_Game_Main* game)
{
	JG_Game_HUD * HUD = new JG_Game_HUD();
	if (HUD && HUD->init(game))
	{
		HUD->autorelease();
		return HUD;
	}
	CC_SAFE_DELETE(HUD);
	return NULL;
}


bool JG_Game_HUD::init(JG_Game_Main* game)
{
	mainGame=game;

	lifeTexture_Active =CCTextureCache::sharedTextureCache()->addImage("heart_active.png");
	lifeTexture_Active->retain();

	lifeTexture_Diactive = CCTextureCache::sharedTextureCache()->addImage("heart_deactive.png");
	lifeTexture_Diactive->retain();

	lifeDrawPosition = ccp(game->screenSize.width * 0.05
		,game->screenSize.height-(lifeTexture_Active->getContentSizeInPixels().height * 1.1 ));

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

	debugLabel = CCLabelTTF::create();
	this->addChild(debugLabel);
	debugLabel->setScale(2);
	debugLabel->setColor(ccWHITE);
	debugLabel->setPosition( mainGame->screenSize * 0.8);

	balldepict = CCLabelTTF::create();
	handdepict = CCLabelTTF::create();

	this->addChild(balldepict);
	balldepict->setScale(2);
	balldepict->setColor(ccWHITE);
	balldepict->setPosition( mainGame->screenSize * 0.3);

	this->addChild(handdepict);
	handdepict->setScale(2);
	handdepict->setColor(ccWHITE);
	handdepict->setPosition(mainGame->screenSize * 0.5);
	
	prevballcount = CCLabelTTF::create();

	this->addChild(prevballcount);
	prevballcount->setScale(2);
	prevballcount->setColor(ccWHITE);
	prevballcount->setPosition(mainGame->screenSize * 0.6);
	

	Init_PauseMenu();

	return true;

}

void JG_Game_HUD::Init_PauseMenu()
{		
	gameMenu = CCMenu::create();
	this->addChild(gameMenu);
	gameMenu->setPosition(CCPointZero);

	gameMenu->retain();

	pauseButton = CCMenuItemSprite::create(CCSprite::create("Pause_Normal.png"),CCSprite::create("Pause_Selected.png")
		 ,mainGame
		,menu_selector(JG_Game_Main::PauseGame));
	pauseButton->retain();
	pauseButton->setPosition(ccp(mainGame->screenSize.width * 0.85 ,mainGame->screenSize.height * 0.90) );

	resumeButton = CCMenuItemSprite::create(CCSprite::create("Resume_Normal.png"),CCSprite::create("Resume_Selected.png")
		 ,mainGame
		 ,menu_selector(JG_Game_Main::ResumeGame));
	resumeButton->retain();
	resumeButton->setPosition(ccp(mainGame->screenSize.width * 0.5 ,mainGame->screenSize.height * 0.7));

	resetButton = CCMenuItemSprite::create(CCSprite::create("Reset_Normal.png"),CCSprite::create("Reset_Selected.png")
		 ,mainGame
		 ,menu_selector(JG_Game_Main::ResetGame));
	resetButton->retain();
	resetButton->setPosition(ccp(mainGame->screenSize.width * 0.5 ,mainGame->screenSize.height * 0.5));


	exitButton = CCMenuItemSprite::create(CCSprite::create("Exit_Normal.png"),CCSprite::create("Exit_Selected.png")
		 ,mainGame
		 ,menu_selector(JG_Game_Main::ExitGame));
	exitButton->retain();
	exitButton->setPosition(ccp(mainGame->screenSize.width * 0.5 ,mainGame->screenSize.height * 0.3));

	ballAddButton = CCMenuItemSprite::create(CCSprite::create("BallAdder_Normal.png"),CCSprite::create("BallAdder_Selected.png")
		 ,mainGame
		 ,menu_selector(JG_Game_Main::ReleaseBall));
	ballAddButton->setOpacity(170);
	ballAddButton->retain();
	ballAddButton->setPosition(ccp(mainGame->screenSize.width * 0.1 ,mainGame->screenSize.height * 0.8));


	
	gameMenu->addChild(pauseButton);
	gameMenu->addChild(resumeButton);
	gameMenu->addChild(resetButton);
	gameMenu->addChild(exitButton);
	gameMenu->addChild(ballAddButton);
	
	ShowPauseScreen(false);

}


void JG_Game_HUD::ShowPauseScreen(bool bShow)
{
	pauseButton->setVisible(!bShow);
	resumeButton->setVisible(bShow);
	resetButton->setVisible(bShow);
	exitButton->setVisible(bShow);	
}

void JG_Game_HUD::ShowEndRoundScreen(bool bShow)
{
	
	resetButton->setVisible(bShow);
	exitButton->setVisible(bShow);
	pauseButton->setVisible(!bShow);
}
JG_Game_HUD::~JG_Game_HUD(void)
{
}

void JG_Game_HUD::draw()
{
	if(mainGame!=NULL)
	{
		//DrawLife();
	}
}


void JG_Game_HUD::DrawLife()
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



void JG_Game_HUD::UpdateScore()
{
	scoreLabel->setString(CCString::createWithFormat("%i", mainGame->score)->getCString());
	scoreLabel->runAction((CCAction *)ScoreGainAnimation->copy());
}

void JG_Game_HUD::UpdateReservedBall()
{
	reservedBallLabel->setString(CCString::createWithFormat("%i", mainGame->reservedBallCount)->getCString());
	reservedBallLabel->runAction((CCAction *)ScoreGainAnimation->copy());

}