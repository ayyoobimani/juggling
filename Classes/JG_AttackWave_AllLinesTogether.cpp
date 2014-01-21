#include "JG_AttackWave_AllLinesTogether.h"


JG_AttackWave_AllLinesTogether::JG_AttackWave_AllLinesTogether(void)
{
}


JG_AttackWave_AllLinesTogether::~JG_AttackWave_AllLinesTogether(void)
{
}

void JG_AttackWave_AllLinesTogether::initAttackWave(float attackDifficulty, int attackCount)
{

	JG_AttackWave_Base::initAttackWave( attackDifficulty, attackCount);

	enemeyAddInterval = 0.3;
	pathCounter =0;
	

	int currentTotalPathExist = CALL_MEMBER_FN(listenerObj,getAvailablePathCountFunction)();

	float totalDifficulty = 0.0;
	float currentEnemyDifficulty;




	while( totalDifficulty < attackDifficulty)
	{


		currentEnemy=addEnemy();

		
		//CCLOG("enemy added");
		currentEnemyDifficulty = currentEnemy->GetDifficulty();
		
		//difficulty added because of path health
		currentEnemyDifficulty *= (1 + ( (100 - selectPath(pathCounter)->GetHealth() )/100 ));

		//defficulty added because of concurrent path attack
		currentEnemyDifficulty *= ( ( (pathCounter+1) > currentTotalPathExist ? currentTotalPathExist:(pathCounter+1)) *2 );
		
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

void JG_AttackWave_AllLinesTogether::update(float dt)
{
	//CCLOG("UPDATE");
}

JG_Enemy_Base* JG_AttackWave_AllLinesTogether::addEnemy()
{
	JG_Enemy_Base* tempEnemy;
	std::vector<SEnemyTypes> enemyTypes = CALL_MEMBER_FN(listenerObj, getEnemyTypesFunction)();
	tempEnemy = (JG_Enemy_Base*) enemyTypes[selectEnemyType()].factory->Create();
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	tempEnemy->InitialEnemy(ccp((float)(- 20),(float) screenSize.height + 20.0), EnemyBonus_None);
	
	//mainGame->enemyArray->addObject(tempEnemy);
	CALL_MEMBER_FN(listenerObj,addEnemyFunction)(tempEnemy);
	//remember to initial enemies

	//mainGame->addChild((CCNode*) tempEnemy,50);
	pathQueue.push(pathCounter);
	enemyQueue.push(tempEnemy);
	return tempEnemy;
}

void JG_AttackWave_AllLinesTogether::initiateEnemyAttack(float dt)
{
	//CCLOG("start attack");
	//unschedule(schedule_selector(JG_AttackWave_AllLinesSequential::initiateEnemyAttack));
	//CCLOG(CCString::createWithFormat("enemy path count: %d", pathQueue.size())->getCString());
	JG_Path * enemyPath = selectPath(pathQueue.front());
	pathQueue.pop();
	//CCLOG("moving new enemy");

	if(CALL_MEMBER_FN(listenerObj,getHealthesToRewardCountFunction)() > 0 )
	{
		//CCLOG(CCString::createWithFormat("healths to reward counter is: ", mainGame->getHealthsToRewardCount())->getCString());
		enemyQueue.front()->SetEnemyBonus(EnemyBonus_PathHealth);
		CALL_MEMBER_FN(listenerObj,onHealthRewardedFunction)();
	}
	else if(CALL_MEMBER_FN(listenerObj,getBallsToRewardCountFunction)()>0 )
	{
	//	CCLOG("enemy with ball bonus");
		enemyQueue.front()->SetEnemyBonus(EnemyBonus_ExtraBall);
		CALL_MEMBER_FN(listenerObj,onBallRewardedFunction)();
	}
	enemyQueue.front()->SetDestinationPath(enemyPath->GetPositionForLengthRatio(generateEnemyPositionRatio()),enemyPath);
	enemyQueue.pop();
	//CCLOG("enemt queue ok");
	
	//if more enemie(s) -> schedule next enemy attack
	if(enemyQueue.size()==0)
	{
		CALL_MEMBER_FN(listenerObj,onAttackWaveFinishedFunction)();
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

float JG_AttackWave_AllLinesTogether::generateEnemyPositionRatio()
{
	return (CCRANDOM_0_1()* 0.6) +0.2;
}



JG_Path * JG_AttackWave_AllLinesTogether::selectPath(int pathCount)
{
	JG_Path  * tempPath;
	while(true)
	{
		CCArray* pathsArray = CALL_MEMBER_FN(listenerObj,getPathesArrayFunction)();
		tempPath = (JG_Path*)pathsArray->objectAtIndex(pathCount % pathsArray->count());
		if(tempPath->IsPathEnabled())
		{
			//CCLOG(CCString::createWithFormat("selected paht:%d",pathCount % mainGame->pathsArray->count())->getCString());
			return tempPath;
		}
		pathCount++;
	}
}

