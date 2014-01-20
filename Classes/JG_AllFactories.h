#pragma once
#include "JG_Factory_Base.h"
//#include "JG_Enemy_Base.h"
#include "JG_Factory_Enemy.h"
#include "JG_Enemy_QuickAttakc.h"
#include "JG_Enemy_Crow.h"

#include "JG_AttackWave_Base.h"
#include "JG_AttackWave_AllLinesSequential.h"
#include <vector>

struct EnemyType
{
	JG_Factory_Base* factory;
	int currentChance;
	int chanceIncreasePerRound;
};

using namespace std;
class JG_AllFactories
{
	

public:
	JG_AllFactories(void);
	~JG_AllFactories(void);

	void InitialEnemyTypes();
	void InitialAttackWaveTypes();

	vector<EnemyType>* enemyTypes;
	vector<JG_Factory_Base*>* attackWaveTypes;

	template<class enemyClass>
	EnemyType CreateEnemyType(int baseChance,int chaceIncrease);
};

