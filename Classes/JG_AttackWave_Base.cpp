#include "JG_AttackWave_Base.h"


JG_AttackWave_Base::JG_AttackWave_Base()
{

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

}

void JG_AttackWave_Base::initAttacWave(JG_Game_Main * _mainGame,float _attackDifficulty, int _attackCount)
{
	
	mainGame = _mainGame;
	attackDifficulty = _attackDifficulty;
	attackCount = _attackCount;

	enemyArray = new CCArray();
}