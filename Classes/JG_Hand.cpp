#include "JG_Hand.h"

JG_Hand::JG_Hand()
{

}//adding a comment

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
		hand->radius = game->screenSize.height * RADIUS_RATIO;
		hand->setScale(GAME_SCALE * HAND_SCALE);
		return hand;
	}
	CC_SAFE_DELETE(hand);
	return NULL;

}

void JG_Hand::draw()
{
	CCSprite::draw();

	if(bMustDrawArea)
	{
		ccDrawCircle( convertToNodeSpace(this->getPosition()), radius , 360 , 32 , FALSE);
	}

	//CCLOG("hey there");

}

void JG_Hand::SetAreaVisibility(bool newVisibility)
{
	bMustDrawArea = newVisibility;
}