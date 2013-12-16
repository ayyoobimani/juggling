#pragma once
#include "sprite_nodes\CCSprite.h"
#include "JG_Game_Main.h"
#include "JG_Path.h"

class JG_Path;


// EnemyS_Inited is just for the moment it is inited
enum EEnemyState
{
	EnemyS_Inited,EnemyS_Intending,EnemyS_Attacking,EnemyS_Waiting,EnemyS_Escaping,EnemyS_Dying
};

#define BASE_WAITING_TIME 5
#define BASE_INTERVAL 5
#define FIRST_HIT_COEFFICIENT 2


class JG_Enemy_Base:
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

	JG_Path* targetPath;

	float attackInterval;
	float damagePerSecond;
	float damagePerInterval;
	void Attack(float dt);

	
	void GotoState_Intending();
	void GotoState_Attacking();
	void GotoState_Waiting();
	void GotoState_Escaping();
	void GotoState_Dying();
	
public:
	JG_Enemy_Base(void);
	~JG_Enemy_Base(void);

	//move to  function
	void MoveTo(float dt);
	
	void SetDestinationPath(CCPoint destinationPos, JG_Path * newTargetPath);
	void SetDestinationPosition(CCPoint destinaionPos);
	void CheckOutOfScreen();
	
	void SetState(EEnemyState state);

	void Fall(float dt);

	static JG_Enemy_Base* CreateEnemy(JG_Game_Main* game,CCPoint initialPosition);

	void InitialEnemy(JG_Game_Main* game,CCPoint initialPosition);

	void update(float dt);
	//checking collision with ball
	void CheckCollisionWithBall();


	void ProcessMove(float dt);

	void HandleWaitingToAttacking(float time);

	

	
	//void CheckOutOfScreen();

	

	//float GetScore();

	float  GetDifficulty();
	JG_Path* getPath();
};

