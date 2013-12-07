#pragma once
#include "sprite_nodes\CCSprite.h"
#include "JG_Game_Main.h"

enum EEnemyState
{
	EnemyS_Intending,EnemyS_Attacking,EnemyS_Waiting,EnemyS_Escaping
};


class JG_Enemy:
	public CCSprite
{
private:
	float speed;
	float radius;
	float waitingTime;
	EEnemyState state;
	bool bIsDirectionSet;
	CCPoint destination;
	float delay;

	JG_Game_Main* mainGame;
	float score;

	void SetState(EEnemyState state);

	void IntendingState();
	void AttackingState();
	void WaitingState();
	void EscapingState();

	void BetweenIntendingWaiting(float dt);
public:
	JG_Enemy(void);
	~JG_Enemy(void);

	//move to  function
	void MoveTo(float dt);
	
	void SetDestination(CCPoint destination);
	


	static JG_Enemy* CreateEnemy(JG_Game_Main* game,CCPoint initialPosition,float initialSpeed,float delay);
	void update(float dt);
	//checking collision with ball
	void CheckCollisionWithBall();

	void ProcessMove(float dt);

	

	
	//void CheckOutOfScreen();

	

	//float GetScore();

	
};

