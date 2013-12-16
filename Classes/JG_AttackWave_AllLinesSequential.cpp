#include "JG_AttackWave_AllLinesSequential.h"


JG_AttackWave_AllLinesSequential::JG_AttackWave_AllLinesSequential()
{

}


JG_AttackWave_AllLinesSequential::~JG_AttackWave_AllLinesSequential(void)
{
}


void JG_AttackWave_AllLinesSequential::initAttackWave( JG_Game_Main* mainGame, float attackDifficulty, int attackCount)
{

	JG_AttackWave_Base::initAttackWave(mainGame, attackDifficulty, attackCount);


	enemeyAddInterval = 3.0 / (attackCount);
	pathCounter =0;
	JG_Path* currentPath;

	float totalDifficulty = 0.0;
	float currentEnemyDifficulty;



	while( totalDifficulty < attackDifficulty)
	{

		CCLOG("adding enemy");                                                
		currentEnemy=addEnemy();

		currentPath = ((JG_Path*) mainGame->pathsArray->objectAtIndex( pathCounter % mainGame->pathsArray->count() ) );
		CCLOG("enemy added");
		currentEnemyDifficulty = currentEnemy->GetDifficulty();
		currentEnemyDifficulty *= (1 + ( (100 - currentPath->GetHealth() )/100 ));
		int h = mainGame->enemyArray->count();
		for(int i=0; i<mainGame->enemyArray->count(); i++)
		{
			if(currentPath != (JG_Path*)mainGame->pathsArray->objectAtIndex(i%mainGame->pathsArray->count()) ) 
				currentEnemyDifficulty *= ( 1 + 1.0/((mainGame->enemyArray->count() - i) * enemeyAddInterval) );

		}

		totalDifficulty += currentEnemyDifficulty;

		pathCounter ++;
		//TODO divide by zero exception handling



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
	pathQueue.push(pathCounter);
	enemyQueue.push(tempEnemy);

	initiateEnemyAttack(2);
	return tempEnemy;
}

void JG_AttackWave_AllLinesSequential::initiateEnemyAttack(float dt)
{
	CCLOG("start attack");

	JG_Path * enemyPath = (JG_Path*)mainGame->pathsArray->objectAtIndex(pathQueue.front() % mainGame->pathsArray->count());
	pathQueue.pop();
	enemyQueue.front()->SetDestinationPath(enemyPath->GetPositionForLengthRatio(generateEnemyPositionRatio()),enemyPath);
	enemyQueue.pop();
	if(enemyQueue.size()>0)
	schedule(schedule_selector(JG_AttackWave_AllLinesSequential::initiateEnemyAttack),0,0,enemeyAddInterval);
}

float JG_AttackWave_AllLinesSequential::generateEnemyPositionRatio()
{
	return CCRANDOM_0_1();
}

