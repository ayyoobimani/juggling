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


	clalculateEnemyAddInterval();
	//CCLOG(CCString::createWithFormat("interval: %f", enemeyAddInterval)->getCString());
	pathCounter =0;
	JG_Path* currentPath;

	float totalDifficulty = 0.0;
	float currentEnemyDifficulty;

	JG_Path * tempPath;


	while( totalDifficulty < attackDifficulty)
	{

		
		currentEnemy=addEnemy();

		currentPath = ((JG_Path*) mainGame->pathsArray->objectAtIndex( pathCounter % mainGame->pathsArray->count() ) );
		CCLOG("enemy added");
		currentEnemyDifficulty = currentEnemy->GetDifficulty();
		currentEnemyDifficulty *= (1 + ( (100 - currentPath->GetHealth() )/100 ));
		int h = mainGame->enemyArray->count();
		for(int i=0; i<enemyQueue.size(); i++)
		{
			if(currentPath !=  selectPath(i) ) 
				currentEnemyDifficulty *= ( 1 + 1.0/((mainGame->enemyArray->count() - i) * enemeyAddInterval) );

		}

		totalDifficulty += currentEnemyDifficulty;

		pathCounter ++;
		//TODO divide by zero exception handling



	}
	//schedule(schedule_selector(JG_AttackWave_AllLinesSequential::initiateEnemyAttack),0,1,0.1);
	//this->schedule(schedule_selector(JG_AttackWave_AllLinesSequential::initiateEnemyAttack),0.5);
	initiateEnemyAttack(2);


}

void JG_AttackWave_AllLinesSequential::update(float dt)
{
	CCLOG("UPDATE");
}

JG_Enemy_Base* JG_AttackWave_AllLinesSequential::addEnemy()
{
	JG_Enemy_Base* tempEnemy;
	tempEnemy = (JG_Enemy_Base*) mainGame->enemyTypes[selectEnemyType()].factory->Create();
	tempEnemy->InitialEnemy(mainGame, ccp(mainGame->screenSize.width,mainGame->screenSize.height));
	mainGame->enemyArray->addObject(tempEnemy);
	//remember to initial enemies

	mainGame->addChild((CCNode*) tempEnemy,50);
	pathQueue.push(pathCounter);
	enemyQueue.push(tempEnemy);
	return tempEnemy;
}

void JG_AttackWave_AllLinesSequential::initiateEnemyAttack(float dt)
{
	CCLOG("start attack");
	unschedule(schedule_selector(JG_AttackWave_AllLinesSequential::initiateEnemyAttack));

	JG_Path * enemyPath = selectPath(pathQueue.front());
	pathQueue.pop();
	enemyQueue.front()->SetDestinationPath(enemyPath->GetPositionForLengthRatio(generateEnemyPositionRatio()),enemyPath);
	enemyQueue.pop();
	if(enemyQueue.size()>0)
	{
		schedule(schedule_selector(JG_AttackWave_AllLinesSequential::initiateEnemyAttack),enemeyAddInterval);
	}
}

float JG_AttackWave_AllLinesSequential::generateEnemyPositionRatio()
{
	return CCRANDOM_0_1();
}

void JG_AttackWave_AllLinesSequential::clalculateEnemyAddInterval()
{
	if(attackCount<31)
		enemeyAddInterval = 3.0 - ( (1.0/15) * attackCount );
	else 
		enemeyAddInterval = 1;
}

JG_Path * JG_AttackWave_AllLinesSequential::selectPath(int pathCount)
{
	JG_Path  * tempPath;
	while(true)
	{
		tempPath = (JG_Path*)mainGame->pathsArray->objectAtIndex(pathCount % mainGame->pathsArray->count());
		if(tempPath->IsPathEnabled())
			return tempPath;

		pathCount++;
	}
}