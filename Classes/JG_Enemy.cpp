#include "JG_Enemy.h"


JG_Enemy::JG_Enemy(void)
{
}


JG_Enemy::~JG_Enemy(void)
{
}

JG_Enemy* JG_Enemy::CreateEnemy(JG_Game_Main* game,CCPoint initialPosition,float initialSpeed,float delay)
{
	
	JG_Enemy * enemy = new JG_Enemy();
	
	if (enemy && enemy->initWithFile("crow.png"))
	{
		enemy->autorelease();
		enemy->setPosition(initialPosition);
		enemy->speed=initialSpeed;
		enemy->scheduleUpdate();
		enemy->mainGame=game;
		enemy->delay=delay;
		return enemy;

	}
	CC_SAFE_DELETE(enemy);
	return NULL;	 
}

void JG_Enemy::CheckCollisionWithBall()
{
	JG_Ball* tempCurrentBall;
	for(int i=0;i<mainGame->GetBallArray()->count();i++)
	{
		tempCurrentBall=(JG_Ball*)mainGame->GetBallArray()->objectAtIndex(i);
		float collision_radius=this->radius+tempCurrentBall->radius;
		if(mainGame->ArePointsColliding(this->getPosition(),tempCurrentBall->getPosition(),collision_radius))
		{
			//mainGame->OnFruitHit(this, tempCurrentBall);
			return;
		}

	}
}
void JG_Enemy::MoveTo(float dt)
{
	this->SetDestination(this->destination);
	float angle=atanf((this->getPositionY()-destination.y)/(this->getPositionX()-destination.x));
	CCPoint temp;
	temp.x=this->getPositionX()+cosf(angle)*speed*dt;
	temp.y=this->getPositionY()+sinf(angle)*speed*dt;
	setPosition(temp);

	if(sqrtf(pow((this->getPositionX()-this->destination.x),2)+pow((this->getPositionY()-this->destination.y),2))<speed)
	{
		this->state=EnemyS_Waiting;
		this->bIsDirectionSet=0;
	}
}

void JG_Enemy::SetDestination(CCPoint destination)
{
	this->destination=destination;
	bIsDirectionSet=true;
}

void JG_Enemy::update(float dt)
{
	if(this->state==EnemyS_Intending&&bIsDirectionSet==true)
		MoveTo(dt);

}
//nonesense
void JG_Enemy::SetState(EEnemyState state)
{
	this->state=state;
}
void JG_Enemy::IntendingState()
{
	SetState(EnemyS_Intending);
	this->schedule(schedule_selector(BetweenIntendingWaiting),0,0,delay);
}
void JG_Enemy::AttackingState()
{
	SetState(EnemyS_Attacking);
}
void JG_Enemy::WaitingState()
{
	SetState(EnemyS_Waiting);
}
void JG_Enemy::EscapingState()
{
	SetState(EnemyS_Escaping);
}
void JG_Enemy::BetweenIntendingWaiting(float dt)
{
	WaitingState();
}