#include "JG_Enemy_QuickAttakc.h"


JG_Enemy_QuickAttakc::JG_Enemy_QuickAttakc(void)
{
	spriteFileName = "Enemies/UFO2/UFO.png";

	attackingAnimationFolder = "Enemies/UFO2/Attacking/";
	dyingAnimationFolder = "Enemies/UFO2/Dying/";
	escapingAnimationFolder = "Enemies/UFO2/Escaping/";
	intendingAnimationFolder = "Enemies/UFO2/Intending/";
	landingAnimationFolder = "Enemies/UFO2/Landing/";
	waitingAnimationFolder = "Enemies/UFO2/Waiting/";
	speed = 500;
	landingTime = 0.5;
	
	attackInterval=1.5;
	damagePerSecond=5;
	damagePerInterval=damagePerSecond*attackInterval;
	CCLOG("finished enemy child constructor");
	
}


JG_Enemy_QuickAttakc::~JG_Enemy_QuickAttakc(void)
{
}

/*
void JG_Enemy_QuickAttakc::InitialEnemy(CCPoint initialPosition)
{
	JG_Enemy_Base::InitialEnemy(initialPosition);
	initWithFile("redcrow.png");
}

void JG_Enemy_QuickAttakc::InitialEnemy(CCPoint initialPosition,EEnemyBonus bonus)
{
	JG_Enemy_Base::InitialEnemy(initialPosition, bonus);
	initWithFile("redcrow.png");

}
*/