#pragma once
#include "sprite_nodes\CCSprite.h"
#include "cocos2d.h"
#include "JG_Factory_Base.h"




using namespace cocos2d;


struct SEnemyTypes
{
	JG_Factory_Base * factory;
	int currentChance;
	int chanceIncreasePerRound;
};


//class JG_Game_Main;

typedef std::vector<SEnemyTypes> (CCObject::*GetEnemyTypesHandler)();
typedef int (CCObject::*GetBallsToRewardCountHandler)();
typedef int (CCObject::*GetHealthesToRewardCountHandler)();
typedef CCArray* (CCObject::*GetPathesArrayHandler)();
typedef void (CCObject::*OnAttackWaveFinishedHandler)();
typedef void (CCObject::*OnHealthRewardedHandler)();
typedef void (CCObject::*OnBallRewardedHandler)();
typedef void (CCObject::*AddEnemyHandler)(JG_Enemy_Base*);
typedef int (CCObject::*GetAvailablePathCountHandler)();

#define CALL_MEMBER_FN(object,ptrToMember)  ((object)->*(ptrToMember))



class JG_AttackWave_Base:public CCNode
{
protected:
	
	

	float attackDifficulty;
	int GetTickCount;
	int attackCount;
	
	int enemyCounter;

	static GetEnemyTypesHandler getEnemyTypesFunction;
	static GetBallsToRewardCountHandler getBallsToRewardCountFunction;
	static GetHealthesToRewardCountHandler getHealthesToRewardCountFunction;
	static GetPathesArrayHandler getPathesArrayFunction;
	static OnAttackWaveFinishedHandler onAttackWaveFinishedFunction;
	static OnHealthRewardedHandler onHealthRewardedFunction;
	static OnBallRewardedHandler onBallRewardedFunction;
	static AddEnemyHandler addEnemyFunction;
	static GetAvailablePathCountHandler getAvailablePathCountFunction;
	static CCObject* listenerObj;
	

public:
	JG_AttackWave_Base();
	~JG_AttackWave_Base(void);

	

	int selectEnemyType();
	virtual void initAttackWave(float attackDifficulty,int attackCount );

	static void SetGetEnemyTypesFunctionPointer(CCObject* obj,GetEnemyTypesHandler);
	static void SetGetBallsToRewardFunctionPointer(CCObject* obj,GetBallsToRewardCountHandler);
	static void SetGetHealthesToRewardCountFunctionPointer(CCObject* obj,GetHealthesToRewardCountHandler);
	static void SetGetPathesArrayFunctionPointer(CCObject* obj,GetPathesArrayHandler);
	static void SetOnAttackWaveFinishedFunctionPointer(CCObject * obj,OnAttackWaveFinishedHandler);
	static void SetOnBallRewardedFunctionPointer(CCObject* obj,OnBallRewardedHandler);
	static void SetOnHealthRewardedFunctionPointer(CCObject* obj, OnHealthRewardedHandler);
	static void SetAddEnemyFunctionPointer(CCObject* obj, AddEnemyHandler);
	static void SetGetAvailablePathCountFunctionPointer(CCObject* obj, GetAvailablePathCountHandler);
};

