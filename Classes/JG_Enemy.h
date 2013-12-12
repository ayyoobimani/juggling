#pragma once
#include "sprite_nodes\CCSprite.h"
#include "JG_Game_Main.h"

enum EEnemyState
{
	EnemyS_Intending,EnemyS_Attacking,EnemyS_Waiting,EnemyS_Escaping
};

#define BASE_WAITING_TIME 5
#define BASE_INTERVAL 5
#define FIRST_HIT_COEFFICIENT 2


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
	float directionRadian;

	JG_Game_Main* mainGame;
	float score;


	float interVal;
	float damagePerSecond;
	float damagePerInterval;
	void Attack();

	
	void GotoState_Intending();
	void GotoState_Attacking();
	void GotoState_Waiting();
	void GotoState_Escaping();



	

	
public:
	JG_Enemy(void);
	~JG_Enemy(void);

	//move to  function
	void MoveTo(float dt);
	
	void SetDestination(CCPoint destination);
	
	void SetState(EEnemyState state);

	static JG_Enemy* CreateEnemy(JG_Game_Main* game,CCPoint initialPosition,float initialSpeed,float delay);
	void update(float dt);
	//checking collision with ball
	void CheckCollisionWithBall();

	void ProcessMove(float dt);

	void HandleWaitingToAttacking(float time);

	

	
	//void CheckOutOfScreen();

	

	//float GetScore();

	float  getDifficulty();
};

