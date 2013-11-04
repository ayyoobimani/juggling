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

	lifeTexture =CCTextureCache::sharedTextureCache()->addImage("heart.png");
	lifeTexture->retain();

	lifeDrawPosition = ccp(10
		,game->screenSize.y-(lifeTexture->getContentSizeInPixels().height +10 ));

	lifeDrawPacing = lifeTexture->getContentSizeInPixels().width + 2;

	scoreLabel =CCLabelBMFont::create ("0", "fonts/font.fnt", mainGame->screenSize.y * 0.3f);
	scoreLabel->setPosition(mainGame->screenSize+ ccp(-60, -20 ));
	this->addChild(scoreLabel);

	ScoreGainAnimation = CCSequence::create(
		CCEaseInOut::create(CCScaleTo::create(0.3,2,2),0.5)
		,CCEaseInOut::create(CCScaleTo::create(0.3,1,1),0.5),NULL);
	ScoreGainAnimation->retain();
	return true;

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
	for(int i = 0 ; i< mainGame->lifeCount ; ++i)
	{
		lifeTexture->drawAtPoint(lifeDrawPosition+ ccp(i*lifeDrawPacing,0));
	}
}

void JG_Game_HUD::UpdateScore()
{
	scoreLabel->setString(CCString::createWithFormat("%i", mainGame->score)->getCString());
	scoreLabel->runAction((CCAction *)ScoreGainAnimation->copy());
}