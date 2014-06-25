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
		hand->throwPower =-1;
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
		glLineWidth(10*mainGame->screenSize.height/320);
		ccDrawColor4B(0, 186, 60, 60); 
		ccDrawCircle( convertToNodeSpace(this->getPosition()), radius , 360 , 32 , false);

	}

}

void JG_Hand::SetAreaVisibility(bool newVisibility)
{
	bMustDrawArea = newVisibility;
}

void JG_Hand::setThrowPower(float _power)
{
	throwPower = _power;
	scheduleOnce(schedule_selector(JG_Hand::resetThrowPower),0.4);
}

void JG_Hand::resetThrowPower(float dt)
{
	throwPower = -1 ;
}

float JG_Hand::getThrowPower()
{
	return throwPower;
}
