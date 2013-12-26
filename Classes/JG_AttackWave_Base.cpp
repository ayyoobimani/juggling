#include "JG_AttackWave_Base.h"
#include "JG_Game_Main.h"


JG_AttackWave_Base::JG_AttackWave_Base()
{
	CCLOG("parent attackwave created");
}


JG_AttackWave_Base::~JG_AttackWave_Base(void)
{
}

int JG_AttackWave_Base::selectEnemyType()
{
	float randomNumber = CCRANDOM_0_1();

	float coefficintSum = 0;

	for(int i=0; i< mainGame->enemyTypes.size() ; i++)
	{
		coefficintSum += mainGame->enemyTypes[i].currentChance;
	}

	
	for(int i=0; i< mainGame->enemyTypes.size() ; i++)
	{
		if (randomNumber <= (mainGame->enemyTypes[i].currentChance / coefficintSum))
		{
			return i;
		}
	}
	
	return 0;
}

void JG_AttackWave_Base::initAttackWave(JG_Game_Main * _mainGame,float _attackDifficulty, int _attackCount)
{
	CCLOG("called parent attack wave init");
	
	mainGame = _mainGame;
	attackDifficulty = _attackDifficulty;
	attackCount = _attackCount;
	//mainGame->addChild(this);

	
}