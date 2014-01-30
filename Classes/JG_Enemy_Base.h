#ifndef __Enemy_Base_H__
#define __Enemy_Base_H__

#include "sprite_nodes\CCSprite.h"
#include "cocos2d.h"
#include "JG_Enums.h"
#include "Defines.h"
#include "actions\CCAction.h"


using namespace cocos2d;

class JG_Path;

class JG_Ball;
class JG_Enemy_Base;

typedef void (CCObject::*OnLostHandler)(JG_Enemy_Base*);
typedef void (CCObject::*OnHitHandler)(JG_Enemy_Base* , JG_Ball*);
typedef void (CCObject::*DamagePathHandler)(JG_Path* , float );
typedef float (CCObject::*GetBallRadiusHandler)(JG_Ball* );
typedef void (CCObject::*PlayMusicHandler)(CCString);
typedef CCArray* (CCObject::*GetBallsHandler)();

#define OnHitSelector(_SELECTOR) (OnHitHandler)(&_SELECTOR)


#define CALL_MEMBER_FN(object,ptrToMember)  ((object)->*(ptrToMember))


// EnemyS_Inited is just for the moment it is inited
enum EEnemyState
{
	EnemyS_Inited, EnemyS_Intending, EnemyS_Attacking
	, EnemyS_Waiting, EnemyS_Escaping, EnemyS_Dying
	, EnemyS_Landing
};

#define BASE_LANDING_TIME 5
#define BASE_INTERVAL 5
#define FIRST_HIT_COEFFICIENT 2


class JG_Enemy_Base:
	public CCSprite
{

private:
	CCSize screenSize;
	static OnLostHandler onLostFunction;
	static OnHitHandler onHitFunction;
	static GetBallsHandler getBallFunction;
	static DamagePathHandler damagePathFunction;
	static GetBallRadiusHandler getBallRadiusFunction;
	static PlayMusicHandler playMusicFunction;
	static CCObject* listenerObj;


	CCAnimate* lastAnimationAction;

	
	float radius;
	
	EEnemyState state;
	EEnemyBonus bonus;
	bool bIsDirectionSet;
	CCPoint destination;
	float directionRadian;

	float score;

	JG_Path* targetPath;


	void Attack();
	


	void SetBonusTexture(EEnemyBonus bonus);
	
	void DrawBonusTexture();

	
	void GotoState_Intending();
	void GotoState_Attacking();
	void GotoState_Waiting();
	void GotoState_Escaping();
	void GotoState_Dying();
	void GotoState_Landing();

	CCAnimation* LoadAnimation(CCString animationFolder, CCString animationName);
	CCAnimation* LoadAnimationFromFolder(CCString animationFolder);

	CCString GetSpriteAddress(CCString folder,int spriteIndex);

protected:
	float attackInterval;
	float damagePerSecond;
	float damagePerInterval;

	float speed;

	float landingTime;

	CCString intendingAnimationFolder;
	CCString attackingAnimationFolder;
	CCString waitingAnimationFolder;
	CCString escapingAnimationFolder;
	CCString dyingAnimationFolder;
	CCString landingAnimationFolder;

	CCString intendingAnimationName;
	CCString attackingAnimationName;
	CCString waitingAnimationName;
	CCString escapingAnimationName;
	CCString dyingAnimationName;
	CCString landingAnimationName;

	

public:
	JG_Enemy_Base(void);
	~JG_Enemy_Base(void);

	static void SetOnLostFunctionPointer(CCObject* rec,OnLostHandler);
	static void SetOnHitFunctionPointer(CCObject* rec, OnHitHandler);
	static void SetGetBallsFunctionPointer(CCObject* obj,GetBallsHandler);
	static void SetDamagePathFunctionPointer(CCObject* obj,DamagePathHandler);
	static void SetGetBallRadiusFunctionPointer(CCObject* obj,GetBallRadiusHandler);
	static void SetPlayMusicFunctionPointer(CCObject* obj,PlayMusicHandler);

	//move to  function
	void MoveTo(float dt);
	
	void SetDestinationPath(CCPoint destinationPos, JG_Path * newTargetPath);
	void SetDestinationPosition(CCPoint destinaionPos);
	void CheckOutOfScreen();
	
	void SetState(EEnemyState state);

	void Fall(float dt);

	static JG_Enemy_Base* CreateEnemy(CCPoint initialPosition);

	virtual void InitialEnemy(CCPoint initialPosition);
	virtual void InitialEnemy(CCPoint initialPosition,EEnemyBonus bonus);

	void update(float dt);
	//checking collision with ball
	void CheckCollisionWithBall();


	void ProcessMove(float dt);

	void HandleWaitingToAttacking(float time);
	void HandleLandingToWaiting(float time);
	void HandleAttackingToWaiting(float time);
	void HandleAttack(float time);

	//animation controlling
	CCAnimation* intendingAnimation;
	CCAnimation* attackingAnimation;
	CCAnimation* waitingAnimation;
	CCAnimation* escapingAnimation;
	CCAnimation* dyingAnimation;
	CCAnimation* landingAnimation;
	//here is the action, we will attack animation to this action to run
	//CCAnimate* animationAction;
	//functions to create upward animation
	virtual void InitialAnimations();
	virtual void InitialIntendingAnimation();
	virtual void InitialAttackingAnimation();
	virtual void InitialWaitingAnimation();
	virtual void InitialEscapingAnimation();
	virtual void InitialDyingAnimation();
	virtual void InitialLandingAnimation();
	//Functions to run actions of the animations 
	void RunAnimation(CCAnimation* animation);
	
	//void CheckOutOfScreen();

	//set texture based on bonus
	
	CCTexture2D* bonusTexture;
	CCString ballBonusSprite;
	CCString healthBonusSprite;
	

	void draw();
	
	EEnemyBonus GetEnemyBonus();
	

	//float GetScore();

	float  GetDifficulty();
	JG_Path* GetTargetPath();

	void SetEnemyBonus(EEnemyBonus bonus);

	CCString spriteFileName;
};

#endif 