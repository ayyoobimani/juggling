#include "JG_Enemy_Crow.h"


JG_Enemy_Crow::JG_Enemy_Crow(void) 
	: JG_Enemy_Base()
{
	spriteFileName = "Enemies/UFO/UFO.png";

}


JG_Enemy_Crow::~JG_Enemy_Crow(void)
{
}

void JG_Enemy_Crow::InitialIntendingAnimation()
{
	JG_Enemy_Base::InitialIntendingAnimation();
	intendingAnimation->addSpriteFrameWithFileName("Enemies/UFO/Intending/1.png");
	intendingAnimation->addSpriteFrameWithFileName("Enemies/UFO/Intending/2.png");
	intendingAnimation->addSpriteFrameWithFileName("Enemies/UFO/Intending/3.png");
	intendingAnimation->addSpriteFrameWithFileName("Enemies/UFO/Intending/4.png");
	intendingAnimation->addSpriteFrameWithFileName("Enemies/UFO/Intending/5.png");

	intendingAnimation->setDelayPerUnit(1.0f/intendingAnimation->getFrames()->count());
	

}

void JG_Enemy_Crow::InitialWaitingAnimation()
{
	JG_Enemy_Base::InitialWaitingAnimation();
	waitingAnimation->addSpriteFrameWithFileName("Enemies/UFO/Waiting/1.png");


	waitingAnimation->setDelayPerUnit(1.0f/waitingAnimation->getFrames()->count());

}

void JG_Enemy_Crow::InitialLandingAnimation()
{
	JG_Enemy_Base::InitialLandingAnimation();
	landingAnimation->addSpriteFrameWithFileName("Enemies/UFO/Landing/1.png");
	landingAnimation->addSpriteFrameWithFileName("Enemies/UFO/Landing/2.png");
	landingAnimation->addSpriteFrameWithFileName("Enemies/UFO/Landing/3.png");
	landingAnimation->addSpriteFrameWithFileName("Enemies/UFO/Landing/4.png");

	landingAnimation->setDelayPerUnit(landingTime/landingAnimation->getFrames()->count());

}

void JG_Enemy_Crow::InitialAttackingAnimation()
{
	JG_Enemy_Base::InitialAttackingAnimation();
	attackingAnimation->addSpriteFrameWithFileName("Enemies/UFO/Attacking/1.png");


	attackingAnimation->setDelayPerUnit(1.0f/attackingAnimation->getFrames()->count());

}

void JG_Enemy_Crow::InitialEscapingAnimation()
{
	JG_Enemy_Base::InitialEscapingAnimation();
	escapingAnimation->addSpriteFrameWithFileName("Enemies/UFO/Escaping/1.png");
	escapingAnimation->addSpriteFrameWithFileName("Enemies/UFO/Escaping/2.png");
	escapingAnimation->addSpriteFrameWithFileName("Enemies/UFO/Escaping/3.png");
	escapingAnimation->addSpriteFrameWithFileName("Enemies/UFO/Escaping/4.png");
	escapingAnimation->addSpriteFrameWithFileName("Enemies/UFO/Escaping/5.png");

	escapingAnimation->setDelayPerUnit(1.0f/escapingAnimation->getFrames()->count());

}

void JG_Enemy_Crow::InitialDyingAnimation()
{
	JG_Enemy_Base::InitialDyingAnimation();
	dyingAnimation->addSpriteFrameWithFileName("Enemies/UFO/Dying/1.png");

	dyingAnimation->setDelayPerUnit(1.0f/dyingAnimation->getFrames()->count());

}



