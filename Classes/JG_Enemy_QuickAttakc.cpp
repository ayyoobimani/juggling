#include "JG_Enemy_QuickAttakc.h"


JG_Enemy_QuickAttakc::JG_Enemy_QuickAttakc(void)
{
	spriteFileName = "Enemies/3Star/mainSprite.png";

	attackingAnimationFolder = "Enemies/UFO2/Attacking/";
	attackingAnimationName = "UFO_2_Attack";

	dyingAnimationFolder = "Enemies/UFO2/Dying/";
	dyingAnimationName = "UFO_2_Die";

	escapingAnimationFolder = "Enemies/UFO2/Escaping/";
	escapingAnimationName = "UFO_2_Escape";

	intendingAnimationFolder = "Enemies/UFO2/Intending/";
	intendingAnimationName = "UFO_2_Intend";

	landingAnimationFolder = "Enemies/UFO2/Landing/";
	landingAnimationName = "UFO_2_Land";

	waitingAnimationFolder = "Enemies/UFO2/Waiting/";
	waitingAnimationName = "UFO_2_Wait";

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