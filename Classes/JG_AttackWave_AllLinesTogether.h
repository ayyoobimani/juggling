#pragma once

#include "JG_AttackWave_Base.h"
#include "JG_Enemy_Base.h"
#include <queue>
#include "JG_Path.h"





using namespace cocos2d;

class JG_Enemy_Base;
//class JG_Game_Main;
class JG_Path;

class JG_AttackWave_AllLinesTogether :
	public JG_AttackWave_Base
{
	float enemeyAddInterval;
	std::queue<int> pathQueue; 
	JG_Enemy_Base* currentEnemy;
	int pathCounter;
	

	std::queue<JG_Enemy_Base *> enemyQueue;

public:
	JG_AttackWave_AllLinesTogether();
	~JG_AttackWave_AllLinesTogether(void);

	void initAttackWave(float attackDifficulty, int attackCount );
	JG_Enemy_Base* addEnemy();
	void initiateEnemyAttack(float dt);
	float generateEnemyPositionRatio();


	JG_Path* selectPath(int pathcount);

	void update(float dt);
	
};

