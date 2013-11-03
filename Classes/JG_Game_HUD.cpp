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

	scoreFont =CCLabelBMFont::create ("0", "fonts/font.fnt", mainGame->screenSize.y * 0.3f);
	scoreFont->setPosition(mainGame->screenSize+ ccp(-60, -15 ));
	this->addChild(scoreFont);

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
		DrawScore();
	}
}

void JG_Game_HUD::DrawLife()
{
	for(int i = 0 ; i< mainGame->lifeCount ; ++i)
	{
		lifeTexture->drawAtPoint(lifeDrawPosition+ ccp(i*lifeDrawPacing,0));
	}
}

void JG_Game_HUD::DrawScore()
{
	scoreFont->setString(CCString::createWithFormat("%i", mainGame->score)->getCString());
}