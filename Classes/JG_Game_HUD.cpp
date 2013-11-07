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

	lifeDrawPosition = ccp(10
		,game->screenSize.y-(lifeTexture_Active->getContentSizeInPixels().height +10 ));

	lifeDrawPacing = lifeTexture_Active->getContentSizeInPixels().width + 2;

	scoreLabel =CCLabelBMFont::create ("0", "fonts/font.fnt", mainGame->screenSize.y * 0.3f);
	scoreLabel->setPosition(mainGame->screenSize+ ccp(-60, -60 ));
	this->addChild(scoreLabel);

	ScoreGainAnimation = CCSequence::create(
		CCEaseInOut::create(CCScaleTo::create(0.3,2,2),0.5)
		,CCEaseInOut::create(CCScaleTo::create(0.3,1,1),0.5),NULL);
	ScoreGainAnimation->retain();


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
	pauseButton->setPosition(mainGame->screenSize+ ccp(-60, -20 ));

	resumeButton = CCMenuItemSprite::create(CCSprite::create("Resume_Normal.png"),CCSprite::create("Resume_Selected.png")
		 ,mainGame
		 ,menu_selector(JG_Game_Main::ResumeGame));
	resumeButton->retain();
	resumeButton->setPosition(mainGame->screenSize/2 + ccp(0,40));

	resetButton = CCMenuItemSprite::create(CCSprite::create("Reset_Normal.png"),CCSprite::create("Reset_Selected.png")
		 ,mainGame
		 ,menu_selector(JG_Game_Main::ResetGame));
	resetButton->retain();
	resetButton->setPosition(mainGame->screenSize/2 + ccp(0,0));


	exitButton = CCMenuItemSprite::create(CCSprite::create("Exit_Normal.png"),CCSprite::create("Exit_Selected.png")
		 ,mainGame
		 ,menu_selector(JG_Game_Main::ExitGame));
	exitButton->retain();
	exitButton->setPosition(mainGame->screenSize/2 + ccp(0,-40));


	
	gameMenu->addChild(pauseButton);
	gameMenu->addChild(resumeButton);
	gameMenu->addChild(resetButton);
	gameMenu->addChild(exitButton);
	
	SetPauseScreen(false);

}


void JG_Game_HUD::SetPauseScreen(bool bShow)
{

	pauseButton->setVisible(!bShow);
	resumeButton->setVisible(bShow);
	resetButton->setVisible(bShow);
	exitButton->setVisible(bShow);	
}
JG_Game_HUD::~JG_Game_HUD(void)
{
}

void JG_Game_HUD::draw()
{
	if(mainGame!=NULL)
	{
		DrawLife();
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