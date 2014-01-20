#include "JG_Enemy_QuickAttakc.h"


JG_Enemy_QuickAttakc::JG_Enemy_QuickAttakc(void)
{
	spriteFileName = "crow.png";
	speed = 500;
	waitingTime = 0.5;
	
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