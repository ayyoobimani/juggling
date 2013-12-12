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
		enemy->waitingTime=delay;
		enemy->state = EnemyS_Waiting;
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
	//this->SetDestination(this->destination);
	if( getPosition().getDistance(destination)< speed *dt)
	{
		
		setPosition(destination);
		bIsDirectionSet=false;
		SetState(EnemyS_Waiting);
		return;
	}
	CCPoint temp;
	temp.x=this->getPositionX()+cosf(directionRadian)*speed*dt;
	temp.y=this->getPositionY()+sinf(directionRadian)*speed*dt;
	setPosition(temp);

}

void JG_Enemy::SetDestination(CCPoint destination)
{
	this->destination=destination;
	bIsDirectionSet=true;
	CCPoint direction = destination- getPosition();

	// calculate the direction in Radian
	//********************* TODO: find a better soloution for this ****************/
	directionRadian = atan(direction.y/direction.x);
	if(direction.x <0 )
		directionRadian+= CC_DEGREES_TO_RADIANS(180);

	//if(directionRad<0)
		//directionRad+= CC_DEGREES_TO_RADIANS(360);

	CCLog("Direction is : %f ", CC_RADIANS_TO_DEGREES(directionRadian));
	SetState(EnemyS_Intending);
}

void JG_Enemy::update(float dt)
{
	if(state==EnemyS_Intending&&bIsDirectionSet==true)
		MoveTo(dt);

}
//nonesense
void JG_Enemy::SetState(EEnemyState newState)
{
	this->unschedule(schedule_selector(JG_Enemy::HandleWaitingToAttacking));
	switch (newState)
	{
	case EnemyS_Attacking:
		GotoState_Attacking();
		break;
	case EnemyS_Escaping:
		GotoState_Escaping();
		break;
	case EnemyS_Intending:
		GotoState_Intending();
		break;
	case EnemyS_Waiting:
		GotoState_Waiting();
		break;
	}
	state=newState;
}
void JG_Enemy::GotoState_Intending()
{
	CCLog("In state Intending");
	
	
}
void JG_Enemy::GotoState_Attacking()
{
	CCLog("In state Attacking");
	

}
void JG_Enemy::GotoState_Waiting()
{
	CCLog("In state Waiting");
	

	this->schedule(schedule_selector(JG_Enemy::HandleWaitingToAttacking),0,0,waitingTime);
	
}
void JG_Enemy::GotoState_Escaping()
{
	CCLog("In state Escaping");

	
	
}
void JG_Enemy::HandleWaitingToAttacking(float dt)
{
	SetState(EnemyS_Attacking);
}
void JG_Enemy::Attack()
{
	damagePerInterval=damagePerSecond*interVal;
}