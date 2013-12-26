#pragma once
#include "sprite_nodes\CCSprite.h"
#include "JG_Game_Main.h"
#include "JG_Path.h"

class JG_Path;

enum EEnemyBonus;
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
	EEnemyBonus bonus;
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

	void SetBonusTexture(EEnemyBonus bonus);
	void SetEnemyBonus(EEnemyBonus bonus);
	void DrawBonusTexture();

	
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

	virtual void InitialEnemy(JG_Game_Main* game,CCPoint initialPosition);
	virtual void InitialEnemy(JG_Game_Main* game,CCPoint initialPosition,EEnemyBonus bonus);

	void update(float dt);
	//checking collision with ball
	void CheckCollisionWithBall();


	void ProcessMove(float dt);

	void HandleWaitingToAttacking(float time);

	//animation controlling
	CCAnimation* intendingAnimation;
	CCAnimation* attackingAnimation;
	CCAnimation* waitingAnimation;
	CCAnimation* escapingAnimation;
	CCAnimation* dyingAnimation;
	//here is the action, we will attack animation to this action to run
	CCAnimate* animationAction;
	//functions to create upward animation
	void InitialIntendingAnimation();
	void InitialAttackingAnimation();
	void InitialWaitingAnimation();
	void InitialEscapingAnimation();
	void InitialDyingAnimation();
	//Functions to run actions of the animations 
	void RunAnimation(CCAnimation* animation);
	
	//void CheckOutOfScreen();

	//set texture based on bonus
	CCTexture2D* ballBonusTexture;
	

	void draw();
	
	EEnemyBonus GetEnemyBonus();
	

	//float GetScore();

	float  GetDifficulty();
	JG_Path* GetTargetPath();
};

