#include "JG_Enemy_Base.h"


JG_Enemy_Base::JG_Enemy_Base(void)
{
	speed = 300;
	waitingTime = 1.0;
	SetState(EnemyS_Inited);
	targetPath = NULL;
	attackInterval=0.5;
	damagePerSecond=10;
	damagePerInterval=damagePerSecond*attackInterval;
	radius=15;
}


JG_Enemy_Base::~JG_Enemy_Base(void)
{
}

JG_Enemy_Base* JG_Enemy_Base::CreateEnemy(JG_Game_Main* game,CCPoint initialPosition)
{
	
	JG_Enemy_Base * enemy = new JG_Enemy_Base();
	
	if (enemy )
	{
		enemy->InitialEnemy(game,initialPosition);
		
		return enemy;

	}
	CC_SAFE_DELETE(enemy);
	return NULL;	 
}

void JG_Enemy_Base::InitialEnemy(JG_Game_Main* game,CCPoint initialPosition)
{
	initWithFile("crow.png");
	autorelease();
	setPosition(initialPosition);
	scheduleUpdate();
	mainGame=game;

}

void JG_Enemy_Base::MoveTo(float dt)
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

void JG_Enemy_Base::SetDestinationPath(CCPoint destination, JG_Path * newTargetPath)
{
	SetDestinationPosition(destination);
	targetPath = newTargetPath;
	
	SetState(EnemyS_Intending);
}

void JG_Enemy_Base::SetDestinationPosition(CCPoint destinaionPos)
{
	this->destination=destinaionPos;
	
	bIsDirectionSet=true;
	CCPoint direction = destination- getPosition();

	// calculate the direction in Radian
	//********************* TODO: find a better soloution for this ****************/
	directionRadian = atan(direction.y/direction.x);
	if(direction.x <0 )
		directionRadian+= CC_DEGREES_TO_RADIANS(180);

	//if(directionRad<0)
		//directionRad+= CC_DEGREES_TO_RADIANS(360);

	//CCLog("Direction is : %f ", CC_RADIANS_TO_DEGREES(directionRadian));
	
}

void JG_Enemy_Base::Fall(float dt)
{
	speed+=GRAVITY*dt;
	setPositionY(-speed*dt+getPositionY());
}

void JG_Enemy_Base::update(float dt)
{
	if((state==EnemyS_Intending || state==EnemyS_Escaping) &&bIsDirectionSet==true)
		MoveTo(dt);

	CheckCollisionWithBall();
	CheckOutOfScreen();

	if(state==EnemyS_Dying)
		Fall(dt);

}
//nonesense
void JG_Enemy_Base::SetState(EEnemyState newState)
{
	this->unschedule(schedule_selector(JG_Enemy_Base::HandleWaitingToAttacking));
	this->unschedule(schedule_selector(JG_Enemy_Base::Attack));
	state=newState;
	switch (newState)
	{
	case EnemyS_Inited:
		break;
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
	case EnemyS_Dying:
		GotoState_Dying();
		break;
	}
	
}
void JG_Enemy_Base::GotoState_Intending()
{
	//CCLog("In state Intending");
	
	
}
void JG_Enemy_Base::GotoState_Attacking()
{
	//CCLog("In state Attacking");
	this->schedule(schedule_selector(JG_Enemy_Base::Attack),attackInterval);
	

}
void JG_Enemy_Base::GotoState_Waiting()
{
	//CCLog("In state Waiting");
	if(targetPath!=NULL)
		this->schedule(schedule_selector(JG_Enemy_Base::HandleWaitingToAttacking),0,0,waitingTime);
	
}
void JG_Enemy_Base::GotoState_Escaping()
{
	//CCLog("In state Escaping");
	SetDestinationPosition(ccp(mainGame->screenSize.width+10,mainGame->screenSize.height+300));
	targetPath = NULL;


	
	
}
void JG_Enemy_Base::GotoState_Dying()
{
	//CCLog("In state dying");
	//it is falling so the initial speed is zero
	speed=0;
	
}
void JG_Enemy_Base::HandleWaitingToAttacking(float dt)
{
	SetState(EnemyS_Attacking);
}
void JG_Enemy_Base::Attack(float dt)
{
	if(targetPath!=NULL)
		targetPath->TakeDamage(damagePerInterval);
}

float JG_Enemy_Base::GetDifficulty()
{
	float waitingTimeFactor;
	float intervalFactor;
	if(BASE_WAITING_TIME >= waitingTime +1)
		waitingTimeFactor = (BASE_WAITING_TIME - waitingTime);
	else
		waitingTimeFactor = 1;

	if(BASE_INTERVAL >= attackInterval +1)
		intervalFactor = (BASE_INTERVAL - attackInterval);
	else
		intervalFactor = 1;

	//return ( ( waitingTimeFactor * FIRST_HIT_COEFFICIENT + intervalFactor )*damagePerInterval ) ;

	return 70;
}

JG_Path* JG_Enemy_Base::GetTargetPath()
{
	return targetPath;
}

void JG_Enemy_Base::CheckCollisionWithBall()
{
	JG_Ball* tempCurrentBall;
	for(int i=0;i<mainGame->GetBallArray()->count();i++)
	{
		tempCurrentBall=(JG_Ball*)mainGame->GetBallArray()->objectAtIndex(i);
		float collision_radius=this->radius+tempCurrentBall->radius;
		if(mainGame->ArePointsColliding(this->getPosition(),tempCurrentBall->getPosition(),collision_radius))
		{
			mainGame->OnEnemyHit(this, tempCurrentBall);
			return;
		}

	}
}
//TODO:clean this nonesense
void JG_Enemy_Base::CheckOutOfScreen()
{
	
	if(state==EnemyS_Escaping)
		if( getPositionY() < -20 || getPositionX() < -20 || getPositionX() > mainGame->screenSize.width + 20||getPositionY()<mainGame->screenSize.height+30)
			mainGame->OnEnemyLost(this);
	else
	{
		if( getPositionY() < -20 || getPositionX() < -20 || getPositionX() > mainGame->screenSize.width + 20)
			mainGame->OnEnemyLost(this);
	}

}