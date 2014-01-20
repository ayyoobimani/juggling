#include "JG_AllFactories.h"


JG_AllFactories::JG_AllFactories(void)
{
}


JG_AllFactories::~JG_AllFactories(void)
{
}

void JG_AllFactories::InitialEnemyTypes()
{
	enemyTypes = new vector<EnemyType>;
	enemyTypes->push_back(CreateEnemyType<JG_Enemy_Base>(6,2));
	enemyTypes->push_back(CreateEnemyType<JG_Enemy_QuickAttakc>(1,3));
}

void JG_AllFactories::InitialAttackWaveTypes()
{
	attackWaveTypes = new vector<JG_Factory_Base*>;
	attackWaveTypes->push_back(new JG_Factory_Enemy<JG_AttackWave_AllLinesSequential>);
}

template<class enemyClass>
EnemyType JG_AllFactories::CreateEnemyType(int baseChance,int chaceIncrease)
{
	EnemyType enemyType;
	enemyType.factory = new JG_Factory_Enemy<enemyClass>;
	enemyType.currentChance = baseChance;
	enemyType.chanceIncreasePerRound = chaceIncrease;
	return enemyType;
}
