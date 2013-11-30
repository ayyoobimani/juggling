#pragma once
#include "sprite_nodes\CCSprite.h"
#include "JG_Game_Main.h"
#include "cocos2d.h"
using namespace cocos2d;

class JG_Game_Main;

class JG_Fruit :
	public CCSprite
{
private:
	float speed;
	float radius;
	JG_Game_Main* mainGame;
	float score;

public:
	JG_Fruit(void);
	~JG_Fruit(void);

	static JG_Fruit* CreateFruit(JG_Game_Main* game,CCPoint initialPosition,float initialSpeed);
	void update(float dt);
	//checking collision with ball
	void CheckCollisionWithBall();

	void ProcessMove(float dt);
	void CheckOutOfScreen();

	float GetScore();
};

