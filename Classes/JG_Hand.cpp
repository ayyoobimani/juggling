#include "JG_Hand.h"

JG_Hand::JG_Hand()
{

}

JG_Hand::~JG_Hand()
{

}

JG_Hand* JG_Hand::CreateHand(JG_Game_Main* game, CCPoint initialPos, const char * handSprite)
{
	JG_Hand * hand = new JG_Hand();
	if (hand && hand->initWithFile(handSprite))
	{
		hand->autorelease();
		hand->setPosition(initialPos);
		hand->mainGame = game;
		hand->radius = game->screenSize.height * 0.3;
		return hand;
	}
	CC_SAFE_DELETE(hand);
	return NULL;

}

