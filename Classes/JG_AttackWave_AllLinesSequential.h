#pragma once
#include "JG_AttackWave_Base.h"
#include "JG_Enemy_Base.h"
//#include "JG_Game_Main.h"




using namespace cocos2d;

class JG_Enemy_Base;
class JG_Game_Main;

class JG_AttackWave_AllLinesSequential: public JG_AttackWave_Base
{
	float enemeyAddInterval;
	int pathCounter;
	JG_Enemy_Base* currentEnemy;

public:
	JG_AttackWave_AllLinesSequential();
	~JG_AttackWave_AllLinesSequential(void);

	void initAttackWave(JG_Game_Main *_mainGame, float attackDifficulty, int attackCount );
	JG_Enemy_Base* addEnemy();
	void initiateEnemyAttack(float dt);
	float generateEnemyPositionRatio();
};

