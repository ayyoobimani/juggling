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

	int currentTotalPathExist = mainGame->getAvailablePathCount();

	float totalDifficulty = 0.0;
	float currentEnemyDifficulty;

	JG_Path * tempPath;


	while( totalDifficulty < attackDifficulty)
	{


		currentEnemy=addEnemy();

		
		//CCLOG("enemy added");
		currentEnemyDifficulty = currentEnemy->GetDifficulty();
		
		//difficulty added because of path health
		currentEnemyDifficulty *= (1 + ( (100 - selectPath(pathCounter)->GetHealth() )/100 ));

		//defficulty added because of concurrent path attack
		currentEnemyDifficulty *= pathCounter > currentTotalPathExist ? currentTotalPathExist:pathCounter;
		
		totalDifficulty += currentEnemyDifficulty;

		enemyCounter ++;
		pathCounter ++;
		//TODO pathcounter is redundant



	}
	//schedule(schedule_selector(JG_AttackWave_AllLinesSequential::initiateEnemyAttack),0,1,0.1);
	//this->schedule(schedule_selector(JG_AttackWave_AllLinesSequential::initiateEnemyAttack),0.5);
	//initiateEnemyAttack(2);
	//schedule(schedule_selector(JG_AttackWave_AllLinesSequential::initiateEnemyAttack),enemeyAddInterval,pathCo);
	schedule(schedule_selector(JG_AttackWave_AllLinesSequential::initiateEnemyAttack), enemeyAddInterval, pathCounter-1,0);


}

void JG_AttackWave_AllLinesSequential::update(float dt)
{
	//CCLOG("UPDATE");
}

JG_Enemy_Base* JG_AttackWave_AllLinesSequential::addEnemy()
{
	JG_Enemy_Base* tempEnemy;
	tempEnemy = (JG_Enemy_Base*) mainGame->enemyTypes[selectEnemyType()].factory->Create();
	tempEnemy->InitialEnemy(mainGame, ccp(mainGame->screenSize.width,mainGame->screenSize.height), EnemyBonus_None);
	mainGame->enemyArray->addObject(tempEnemy);
	//remember to initial enemies

	mainGame->addChild((CCNode*) tempEnemy,50);
	pathQueue.push(pathCounter);
	enemyQueue.push(tempEnemy);
	return tempEnemy;
}

void JG_AttackWave_AllLinesSequential::initiateEnemyAttack(float dt)
{
	//CCLOG("start attack");
	//unschedule(schedule_selector(JG_AttackWave_AllLinesSequential::initiateEnemyAttack));
	//CCLOG(CCString::createWithFormat("enemy path count: %d", pathQueue.size())->getCString());
	JG_Path * enemyPath = selectPath(pathQueue.front());
	pathQueue.pop();
	//CCLOG("path queue ok");
	enemyQueue.front()->SetDestinationPath(enemyPath->GetPositionForLengthRatio(generateEnemyPositionRatio()),enemyPath);
	enemyQueue.pop();
	//CCLOG("enemt queue ok");
	
	//if more enemie(s) -> schedule next enemy attack
	if(enemyQueue.size()==0)
	{
		mainGame->initiateNewAttackWave();
		//CCLOG("enemy queue is ok");
		//schedule(schedule_selector(JG_AttackWave_AllLinesSequential::initiateEnemyAttack),0,0,enemeyAddInterval);
		//scheduleOnce(schedule_selector(JG_AttackWave_AllLinesSequential::initiateEnemyAttack), enemeyAddInterval);
//		schedule(schedule_selector(JG_AttackWave_AllLinesSequential::initiateEnemyAttack),enemeyAddInterval);

	}
	//if no more enemies -> tell the main game that attack has finished
	//else
	//{
		
	//}
}

float JG_AttackWave_AllLinesSequential::generateEnemyPositionRatio()
{
	return (CCRANDOM_0_1()* 0.6) +0.2;
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
		{
			//CCLOG(CCString::createWithFormat("selected paht:%d",pathCount % mainGame->pathsArray->count())->getCString());
			return tempPath;
		}
		pathCount++;
	}
}

