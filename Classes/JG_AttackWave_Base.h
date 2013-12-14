#pragma once
#include "sprite_nodes\CCSprite.h"
#include "cocos2d.h"
//#include "JG_Game_Main.h"




using namespace cocos2d;

class JG_Game_Main;



class JG_AttackWave_Base:public CCSprite
{
protected:
	JG_Game_Main* mainGame;

	CCArray* enemyArray;

	float attackDifficulty;
	int GetTickCount;
	int attackCount;

public:
	JG_AttackWave_Base();
	~JG_AttackWave_Base(void);

	

	int selectEnemyType();
	virtual void initAttacWave(JG_Game_Main * _mainGame, float attackDifficulty,int attackCount );
};

