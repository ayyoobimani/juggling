#include "JG_AttackWave_AllLinesSequential.h"


JG_AttackWave_AllLinesSequential::JG_AttackWave_AllLinesSequential()
{
	
}


JG_AttackWave_AllLinesSequential::~JG_AttackWave_AllLinesSequential(void)
{
}


void JG_AttackWave_AllLinesSequential::initAttackWave( JG_Game_Main* mainGame, float attackDifficulty, int attackCount)
{
	CCLOG("called child attack wave init");
	JG_AttackWave_Base::initAttacWave(mainGame, attackDifficulty, attackCount);


	enemeyAddInterval = 3 / (attackCount);
	pathCounter =0;
	
	float totalDifficulty = 0.0;
	float currentEnemyDifficulty;
	
	
	
	while( totalDifficulty < attackDifficulty)
	{
		
						
		currentEnemy=addEnemy();
		currentEnemyDifficulty = currentEnemy->GetDifficulty();
		currentEnemyDifficulty *= (1 + ( (100 - currentEnemy->getPath()->GetHealth() )/100 ));
		for(int i=0; i<mainGame->enemyArray->count(); i++)
		{
			if(currentEnemy->getPath() != ( (JG_Enemy_Base *) mainGame->enemyArray->objectAtIndex(i) )->getPath() ) 
				currentEnemyDifficulty *= 1.0/((mainGame->enemyArray->count() - i) * enemeyAddInterval);

		}

		totalDifficulty += currentEnemyDifficulty;
		 

	}

}

JG_Enemy_Base* JG_AttackWave_AllLinesSequential::addEnemy()
{
	JG_Enemy_Base* tempEnemy;
	tempEnemy = (JG_Enemy_Base*) mainGame->enemyTypes[selectEnemyType()].factory->Create();
	tempEnemy->InitialEnemy(mainGame, ccp(mainGame->screenSize.width,mainGame->screenSize.height));
	mainGame->enemyArray->addObject(tempEnemy);
	//remember to initial enemies
		
	mainGame->addChild((CCNode*) tempEnemy);

	unschedule(schedule_selector(JG_AttackWave_AllLinesSequential::initiateEnemyAttack));
	scheduleOnce(schedule_selector(JG_AttackWave_AllLinesSequential::initiateEnemyAttack),pathCounter*enemeyAddInterval);

	return tempEnemy;
}

void JG_AttackWave_AllLinesSequential::initiateEnemyAttack(float dt)
{
	JG_Path * currentPath = (JG_Path*)mainGame->pathsArray->objectAtIndex(pathCounter);
	
	currentEnemy->SetDestinationPath(currentPath->GetPositionForLengthRatio(generateEnemyPositionRatio()),currentPath);
	pathCounter ++;
	//TODO divide by zero exception handling
	pathCounter %= (mainGame->pathsArray->count()-1);
}

float JG_AttackWave_AllLinesSequential::generateEnemyPositionRatio()
{
	return CCRANDOM_0_1();
}

