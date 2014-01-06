
#include "JG_Enemy_Base.h"


OnLostHandler JG_Enemy_Base::onLostFunction;
OnHitHandler JG_Enemy_Base::onHitFunction;
GetBallsHandler JG_Enemy_Base::getBallFunction;
CCObject* JG_Enemy_Base::listenerObj;

JG_Enemy_Base::JG_Enemy_Base(void)
{
	speed = 300;
	waitingTime = 2.0;
	SetState(EnemyS_Inited);
	targetPath = NULL;
	attackInterval=1.5;
	damagePerSecond=10;
	damagePerInterval=damagePerSecond*attackInterval;
	radius=15;
	SetEnemyBonus(EnemyBonus_None);

	screenSize = CCDirector::sharedDirector()->getWinSize();
}


JG_Enemy_Base::~JG_Enemy_Base(void)
{
}

JG_Enemy_Base* JG_Enemy_Base::CreateEnemy(CCPoint initialPosition)
{
	
	JG_Enemy_Base * enemy = new JG_Enemy_Base();
	
	if (enemy )
	{
		enemy->InitialEnemy(initialPosition);
		
		return enemy;

	}
	CC_SAFE_DELETE(enemy);
	return NULL;	 
}

void JG_Enemy_Base::InitialEnemy(CCPoint initialPosition)
{
	initWithFile("crow.png");
	//here we create animations
	InitialIntendingAnimation();
	InitialAttackingAnimation();
	InitialWaitingAnimation();
	InitialEscapingAnimation();
	InitialDyingAnimation();

	//Bonus Inti
	SetEnemyBonus(EnemyBonus_None);
	
	autorelease();
	setPosition(initialPosition);
	scheduleUpdate();
	

}

void JG_Enemy_Base::InitialEnemy(CCPoint initialPosition,EEnemyBonus bonus)
{
	initWithFile("crow.png");
	//here we create animations
	InitialIntendingAnimation();
	InitialAttackingAnimation();
	InitialWaitingAnimation();
	InitialEscapingAnimation();
	InitialDyingAnimation();

	
	SetEnemyBonus(bonus);
	


	autorelease();
	setPosition(initialPosition);
	scheduleUpdate();

}



void JG_Enemy_Base::SetEnemyBonus(EEnemyBonus bonus)
{
	this->bonus=bonus;
	SetBonusTexture(bonus);
	
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
	//No Need To Do anything
	if(state == newState)
		return;

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
	RunAnimation(intendingAnimation);
	
	
}
void JG_Enemy_Base::GotoState_Attacking()
{
	//CCLog("In state Attacking");
	this->schedule(schedule_selector(JG_Enemy_Base::Attack),attackInterval);
	RunAnimation(attackingAnimation);

}
void JG_Enemy_Base::GotoState_Waiting()
{
	//CCLog("In state Waiting");
	if(targetPath!=NULL)
		this->schedule(schedule_selector(JG_Enemy_Base::HandleWaitingToAttacking),0,0,waitingTime);
	
	RunAnimation(waitingAnimation);
}
void JG_Enemy_Base::GotoState_Escaping()
{
	//CCLog("In state Escaping");
	
	//TODO : Fix this
	//SetDestinationPosition(ccp(mainGame->screenSize.width+10,mainGame->screenSize.height+300));
	SetDestinationPosition(CCPointMake(400,600));
	targetPath = NULL;


	RunAnimation(escapingAnimation);
	
}
void JG_Enemy_Base::GotoState_Dying()
{
	//CCLog("In state dying");
	//it is falling so the initial speed is zero
	speed=0;

	RunAnimation(dyingAnimation);
	
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

	return ( ( waitingTimeFactor * FIRST_HIT_COEFFICIENT + intervalFactor )*damagePerInterval ) ;

	//return 30;
}

JG_Path* JG_Enemy_Base::GetTargetPath()
{
	return targetPath;
}

void JG_Enemy_Base::CheckCollisionWithBall()
{
	if(state ==EnemyS_Dying)
		return;
	JG_Ball* tempCurrentBall;
	CCArray* ballArray = CALL_MEMBER_FN(listenerObj,getBallFunction)();
	for(int i=0;i<ballArray->count();i++)
	{
		tempCurrentBall=(JG_Ball*)ballArray->objectAtIndex(i);
		float collision_radius=this->radius+tempCurrentBall->radius;
		if(JG_Game_Main::ArePointsColliding(this->getPosition(),tempCurrentBall->getPosition(),collision_radius))
		{
			CALL_MEMBER_FN(listenerObj,onHitFunction)(this, tempCurrentBall);
			return;
		}

	}
}
//TODO:clean this nonesense
void JG_Enemy_Base::CheckOutOfScreen()
{
	
	if(state==EnemyS_Escaping)
	{
		if( getPositionY() < -20 || getPositionX() < -20 || getPositionX() > screenSize.width + 20||getPositionY()>screenSize.height+30)
			CALL_MEMBER_FN(listenerObj,onLostFunction)(this);
	}

	else
	{
		if( getPositionY() < -20 || getPositionX() < -20 || getPositionX() > screenSize.width + 20)
			CALL_MEMBER_FN(listenerObj,onLostFunction)(this);
	}

}
/*---------------------------Animation controling--------------------------*/
/*	Animations function
	Here we make animations and we attack files to them*/
void JG_Enemy_Base::InitialIntendingAnimation()
{
	intendingAnimation=CCAnimation::create();
	//loading images from local file system
	//for loop over files
	//intendingAnimation->setDelayPerUnit()
}
void JG_Enemy_Base::InitialAttackingAnimation()
{
	attackingAnimation=CCAnimation::create();
}
void JG_Enemy_Base::InitialWaitingAnimation()
{
	waitingAnimation=CCAnimation::create();
}
void JG_Enemy_Base::InitialEscapingAnimation()
{
	escapingAnimation=CCAnimation::create();
}
void JG_Enemy_Base::InitialDyingAnimation()
{
	dyingAnimation=CCAnimation::create();
}
//function to run animation on the sprite

void JG_Enemy_Base::RunAnimation(CCAnimation* animation)
{
	return;
	animationAction=CCAnimate::create(animation);
	this->runAction(animationAction);

}
/*---------------------------Animation controling--------------------------*/

//drawing texture for enemy
void JG_Enemy_Base::SetBonusTexture(EEnemyBonus bonus)
{

	switch (bonus)
	{
	case EnemyBonus_None:
		ballBonusTexture=NULL;
		break;
	case EnemyBonus_ExtraBall:
		ballBonusTexture=CCTextureCache::sharedTextureCache()->addImage("Bonus_ExtraBall.png");
		break;
	case EnemyBonus_PathHealth:
		ballBonusTexture=CCTextureCache::sharedTextureCache()->addImage("Bonus_PathHealth.png");
		break;
	default:
		break;
	}

}

void JG_Enemy_Base::DrawBonusTexture()
{
	if(ballBonusTexture!=NULL)
		ballBonusTexture->drawAtPoint(ccp(0,0));
}
void JG_Enemy_Base::draw()
{
	CCSprite::draw();
	DrawBonusTexture();
}

EEnemyBonus JG_Enemy_Base::GetEnemyBonus()
{
	return bonus;
}


void JG_Enemy_Base::SetOnLostFunctionPointer(CCObject* obj,OnLostHandler handler)
{
	onLostFunction = handler;
	listenerObj = obj;
}

void JG_Enemy_Base::SetGetBallsFunctionPointer(CCObject* obj,GetBallsHandler handler)
{
	getBallFunction = handler;
	listenerObj = obj;
}

void JG_Enemy_Base::SetOnHitFunctionPointer(CCObject* obj,OnHitHandler handler )
{
	onHitFunction = handler;
	listenerObj = obj;
}