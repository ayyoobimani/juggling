#include "JG_AttackWave_Base.h"
//#include "JG_Game_Main.h"


GetEnemyTypesHandler JG_AttackWave_Base::getEnemyTypesFunction;
GetBallsToRewardCountHandler JG_AttackWave_Base::getBallsToRewardCountFunction;
GetHealthesToRewardCountHandler JG_AttackWave_Base::getHealthesToRewardCountFunction;
GetPathesArrayHandler JG_AttackWave_Base::getPathesArrayFunction;
OnAttackWaveFinishedHandler JG_AttackWave_Base::onAttackWaveFinishedFunction;
OnBallRewardedHandler JG_AttackWave_Base::onBallRewardedFunction;
OnHealthRewardedHandler JG_AttackWave_Base::onHealthRewardedFunction;
AddEnemyHandler JG_AttackWave_Base::addEnemyFunction;
GetAvailablePathCountHandler JG_AttackWave_Base::getAvailablePathCountFunction;
CCObject* JG_AttackWave_Base::listenerObj;

JG_AttackWave_Base::JG_AttackWave_Base()
{
	//CCLOG("parent attackwave created");
}


JG_AttackWave_Base::~JG_AttackWave_Base(void)
{
}

int JG_AttackWave_Base::selectEnemyType()
{
	float randomNumber = CCRANDOM_0_1();

	float coefficintSum = 0;

	std::vector<SEnemyTypes> enemyTypes = CALL_MEMBER_FN(listenerObj, getEnemyTypesFunction)();
	for(int i=0; i< enemyTypes.size() ; i++)
	{
		coefficintSum += enemyTypes[i].currentChance;
	}

	
	for(int i=0; i< enemyTypes.size() ; i++)
	{
		if (randomNumber <= (enemyTypes[i].currentChance / coefficintSum))
		{
			return i;
		}
	}
	
	return 0;
}

void JG_AttackWave_Base::initAttackWave(float _attackDifficulty, int _attackCount)
{
	//CCLOG("called parent attack wave init");
	
	attackDifficulty = _attackDifficulty;
	attackCount = _attackCount;
	//mainGame->addChild(this);

	int enemyCounter =0;
	
}


void JG_AttackWave_Base::SetGetEnemyTypesFunctionPointer(CCObject* obj,GetEnemyTypesHandler handler)
{
	listenerObj = obj;
	getEnemyTypesFunction = handler;
}

void JG_AttackWave_Base::SetGetBallsToRewardFunctionPointer(CCObject* obj,GetBallsToRewardCountHandler handler)
{
	listenerObj = obj;
	getBallsToRewardCountFunction = handler;
}

void JG_AttackWave_Base::SetGetHealthesToRewardCountFunctionPointer(CCObject* obj,GetHealthesToRewardCountHandler handler)
{
	listenerObj = obj;
	getHealthesToRewardCountFunction = handler;
}

void JG_AttackWave_Base::SetGetPathesArrayFunctionPointer(CCObject* obj,GetPathesArrayHandler handler)
{
	listenerObj = obj;
	getPathesArrayFunction = handler;
}

void JG_AttackWave_Base::SetOnAttackWaveFinishedFunctionPointer(CCObject* obj,OnAttackWaveFinishedHandler handler)
{
	listenerObj = obj;
	onAttackWaveFinishedFunction = handler;
}

void JG_AttackWave_Base::SetOnBallRewardedFunctionPointer(CCObject* obj,OnBallRewardedHandler handler)
{
	listenerObj = obj;
	onBallRewardedFunction = handler;
}

void JG_AttackWave_Base::SetOnHealthRewardedFunctionPointer(CCObject* obj,OnHealthRewardedHandler handler)
{
	listenerObj = obj;
	onHealthRewardedFunction = handler;
}

void JG_AttackWave_Base::SetAddEnemyFunctionPointer(CCObject* obj,AddEnemyHandler handler)
{
	listenerObj = obj;
	addEnemyFunction = handler;
}

void JG_AttackWave_Base::SetGetAvailablePathCountFunctionPointer(CCObject* obj, GetAvailablePathCountHandler handler)
{
	listenerObj = obj;
	getAvailablePathCountFunction = handler;
}