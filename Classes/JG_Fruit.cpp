#include "JG_Fruit.h"


JG_Fruit::JG_Fruit(void)
{
	radius=15;
}


JG_Fruit::~JG_Fruit(void)
{
}
void JG_Fruit::update(float dt)
{
	ProcessMove(dt);
	CheckCollisionWithBall();
}

void JG_Fruit::ProcessMove(float dt)
{
	setPositionY(getPositionY()+ dt *speed);
}

JG_Fruit* JG_Fruit::CreateFruit(JG_Game_Main* game,CCPoint initialPosition,float initialSpeed)
{
	JG_Fruit * fruit = new JG_Fruit();
	if (fruit && fruit->initWithFile("fruit.png"))
	{
		fruit->autorelease();
		fruit->setPosition(initialPosition);
		fruit->speed=initialSpeed;
		fruit->scheduleUpdate();
		fruit->mainGame=game;
		return fruit;

	}
	CC_SAFE_DELETE(fruit);
	return NULL;	 
}
void JG_Fruit::CheckCollisionWithBall()
{
	JG_Ball* tempCurrentBall;
	for(int i=0;i<mainGame->GetBallArray()->count();i++)
	{
		tempCurrentBall=(JG_Ball*)mainGame->GetBallArray()->objectAtIndex(i);
		float collision_radius=this->radius+tempCurrentBall->radius;
		if(mainGame->ArePointsColliding(this->getPosition(),tempCurrentBall->getPosition(),collision_radius))
			mainGame->OnFruitHit(tempCurrentBall,this);

	}


}

