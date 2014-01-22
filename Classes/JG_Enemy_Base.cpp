
#include "JG_Enemy_Base.h"


OnLostHandler JG_Enemy_Base::onLostFunction;
OnHitHandler JG_Enemy_Base::onHitFunction;
GetBallsHandler JG_Enemy_Base::getBallFunction;
DamagePathHandler JG_Enemy_Base::damagePathFunction;
GetBallRadiusHandler JG_Enemy_Base::getBallRadiusFunction;

CCObject* JG_Enemy_Base::listenerObj;


JG_Enemy_Base::JG_Enemy_Base(void)
{
	lastAnimationAction= NULL;
	//spriteFileName = "Enemies/UFO/UFO.png";
	speed = 300;
	landingTime = 2.0;
	SetState(EnemyS_Inited);
	targetPath = NULL;
	attackInterval=1.5;
	damagePerSecond=10;
	damagePerInterval=damagePerSecond*attackInterval;
	radius=15;
	SetEnemyBonus(EnemyBonus_None);

	ballBonusSprite = "Bonus_ExtraBall.png";
	healthBonusSprite = "Bonus_PathHealth.png";

	screenSize = CCDirector::sharedDirector()->getWinSize();
	CCLOG("finished enemy father constructor");
}


JG_Enemy_Base::~JG_Enemy_Base(void)
{
	CC_SAFE_RELEASE(intendingAnimation);
	
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
	initWithFile(spriteFileName.getCString());
	//here we create animations
	InitialAnimations();

	//Bonus Inti
	SetEnemyBonus(EnemyBonus_None);
	
	autorelease();
	setPosition(initialPosition);
	scheduleUpdate();
	

}

void JG_Enemy_Base::InitialEnemy(CCPoint initialPosition,EEnemyBonus bonus)
{
	initWithFile(spriteFileName.getCString());
	//here we create animations

	InitialAnimations();

	
	SetEnemyBonus(bonus);
	


	autorelease();
	setPosition(initialPosition);
	scheduleUpdate();

}

void JG_Enemy_Base::InitialAnimations()
{
	

	InitialIntendingAnimation();
	InitialAttackingAnimation();
	InitialWaitingAnimation();
	InitialEscapingAnimation();
	InitialDyingAnimation();
	InitialLandingAnimation();
	setAnchorPoint(ccp(0.5,0.5));
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
		if(state!=EnemyS_Escaping)
			SetState(EnemyS_Landing);
		return;
	}
	CCPoint temp;
	temp.x=this->getPositionX()+cosf(directionRadian)*speed*dt;
	temp.y=this->getPositionY()+sinf(directionRadian)*speed*dt;
	setPosition(temp);

}

void JG_Enemy_Base::SetDestinationPath(CCPoint destination, JG_Path* newTargetPath)
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
	this->unschedule(schedule_selector(JG_Enemy_Base::HandleLandingToWaiting));
	this->unschedule(schedule_selector(JG_Enemy_Base::HandleAttackingToWaiting));
	this->unschedule(schedule_selector(JG_Enemy_Base::HandleAttack));

	//No Need To Do anything
	if(state == newState)
		return;

	
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
	case EnemyS_Landing:
		GotoState_Landing();
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
	RunAnimation(attackingAnimation);

	this->schedule(schedule_selector(JG_Enemy_Base::HandleAttackingToWaiting)
		,0
		,0
		,attackingAnimation->getDuration());

	this->schedule(schedule_selector(JG_Enemy_Base::HandleAttack)
		,0
		,0
		,attackingAnimation->getDuration()/2);
	
	
}
void JG_Enemy_Base::GotoState_Waiting()
{
	//CCLog("In state Waiting");
	if(targetPath!=NULL)
		this->schedule(schedule_selector(JG_Enemy_Base::HandleWaitingToAttacking),0,0,attackInterval);
	
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

void JG_Enemy_Base::GotoState_Landing()
{
	RunAnimation(landingAnimation);
	this->schedule(schedule_selector(JG_Enemy_Base::HandleLandingToWaiting)
		,0
		,0
		,landingTime);
	
}


void JG_Enemy_Base::HandleWaitingToAttacking(float dt)
{
	SetState(EnemyS_Attacking);
}

void JG_Enemy_Base::HandleLandingToWaiting(float dt)
{
	SetState(EnemyS_Waiting);
}

void JG_Enemy_Base::HandleAttackingToWaiting(float dt)
{
	SetState(EnemyS_Waiting);
}

void JG_Enemy_Base::HandleAttack(float dt)
{
	Attack();
}

void JG_Enemy_Base::Attack()
{
	if(targetPath!=NULL)
		CALL_MEMBER_FN(listenerObj,damagePathFunction)(targetPath, damagePerInterval);
	//targetPath->TakeDamage(damagePerInterval);
}

float JG_Enemy_Base::GetDifficulty()
{
	float waitingTimeFactor;
	float intervalFactor;
	if(BASE_LANDING_TIME >= landingTime +1)
		waitingTimeFactor = (BASE_LANDING_TIME - landingTime);
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
		float collision_radius=this->radius+CALL_MEMBER_FN(listenerObj,getBallRadiusFunction)(tempCurrentBall);
		if(ArePointsColliding(this->getPosition(),((CCSprite*)tempCurrentBall)->getPosition(),collision_radius))
		{
			CALL_MEMBER_FN(listenerObj,onHitFunction)(this, tempCurrentBall);
			SetBonusTexture(EnemyBonus_None);
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
		if( getPositionY() < -100 || getPositionX() < -100 || getPositionX() > screenSize.width + 100)
			CALL_MEMBER_FN(listenerObj,onLostFunction)(this);
	}

}
/*---------------------------Animation controling--------------------------*/
/*	Animations function
	Here we make animations and we attack files to them*/
void JG_Enemy_Base::InitialIntendingAnimation()
{
	intendingAnimation=CCAnimation::create();
	intendingAnimation->setLoops(-1);
	intendingAnimation->retain();

	AddSpritesForAnimation(intendingAnimation,intendingAnimationFolder);

	intendingAnimation->setDelayPerUnit(1.0f/intendingAnimation->getFrames()->count());
}

void JG_Enemy_Base::InitialAttackingAnimation()
{
	attackingAnimation=CCAnimation::create();
	attackingAnimation->setRestoreOriginalFrame(true);
	attackingAnimation->retain();

	AddSpritesForAnimation(attackingAnimation,attackingAnimationFolder);

	attackingAnimation->setDelayPerUnit(1.0f/attackingAnimation->getFrames()->count());
}

void JG_Enemy_Base::InitialWaitingAnimation()
{
	waitingAnimation=CCAnimation::create();
	waitingAnimation->setLoops(-1);
	waitingAnimation->retain();

	AddSpritesForAnimation(waitingAnimation,waitingAnimationFolder);

	waitingAnimation->setDelayPerUnit(1.0f/waitingAnimation->getFrames()->count());
}

void JG_Enemy_Base::InitialEscapingAnimation()
{
	escapingAnimation=CCAnimation::create();
	escapingAnimation->setLoops(-1);
	escapingAnimation->retain();

	AddSpritesForAnimation(escapingAnimation,escapingAnimationFolder);

	escapingAnimation->setDelayPerUnit(1.0f/escapingAnimation->getFrames()->count());
}

void JG_Enemy_Base::InitialDyingAnimation()
{
	dyingAnimation=CCAnimation::create();
	dyingAnimation->setLoops(-1);
	dyingAnimation->retain();

	AddSpritesForAnimation(dyingAnimation,dyingAnimationFolder);

	dyingAnimation->setDelayPerUnit(1.0f/dyingAnimation->getFrames()->count());
}

void JG_Enemy_Base::InitialLandingAnimation()
{
	landingAnimation=CCAnimation::create();
	landingAnimation->retain();

	AddSpritesForAnimation(landingAnimation,landingAnimationFolder);

	landingAnimation->setDelayPerUnit(landingTime/landingAnimation->getFrames()->count());
}


void JG_Enemy_Base::AddSpritesForAnimation(CCAnimation* animation, CCString folder)
{
	if( folder.getCString() == "")
		return;
	//TODO: change it to do while
	int i=1;
	CCString spriteAddress = GetSpriteAddress(folder,i);
	while (CCFileUtils::sharedFileUtils()->isFileExist(spriteAddress.getCString()))
	{
		animation->addSpriteFrameWithFileName(spriteAddress.getCString());
		i++;
		spriteAddress = GetSpriteAddress(folder,i);
	}
}

CCString JG_Enemy_Base::GetSpriteAddress(CCString folder,int spriteIndex)
{
	CCString* spriteFileName;
	//TODO: clean this shit
	if(spriteIndex<10)
		spriteFileName = CCString::createWithFormat(+"_0000%d.png",spriteIndex);
	else if(spriteIndex<100)
		spriteFileName = CCString::createWithFormat(+"_000%d.png",spriteIndex);
	else if(spriteIndex<1000)
		spriteFileName = CCString::createWithFormat(+"_00%d.png",spriteIndex);
	
	(*spriteFileName) = folder.m_sString+spriteFileName->m_sString;
	return *spriteFileName;
}
//function to run animation on the sprite

void JG_Enemy_Base::RunAnimation(CCAnimation* animation)
{

	

	CCAnimate* animationAction=CCAnimate::create(animation);
	if(lastAnimationAction != NULL)
		stopAction(lastAnimationAction);
	runAction(animationAction);
	
	lastAnimationAction = animationAction;

}
/*---------------------------Animation controling--------------------------*/

//drawing texture for enemy
void JG_Enemy_Base::SetBonusTexture(EEnemyBonus bonus)
{

	switch (bonus)
	{
	case EnemyBonus_None:
		bonusTexture=NULL;
		break;
	case EnemyBonus_ExtraBall:
		bonusTexture=CCTextureCache::sharedTextureCache()->addImage(ballBonusSprite.getCString());
		break;
	case EnemyBonus_PathHealth:
		bonusTexture=CCTextureCache::sharedTextureCache()->addImage(healthBonusSprite.getCString());
		break;
	default:
		break;
	}

}

void JG_Enemy_Base::DrawBonusTexture()
{
	
	if(bonusTexture!=NULL)
	{
		CCPoint drawPoint = (getTexture()->getContentSize()-bonusTexture->getContentSize())/2;
		drawPoint = drawPoint+ ccp(0,-5);
		bonusTexture->drawAtPoint(drawPoint);
	}
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


void JG_Enemy_Base::SetDamagePathFunctionPointer(CCObject* obj,DamagePathHandler handler )
{
	damagePathFunction = handler;
	listenerObj = obj;
}

void JG_Enemy_Base::SetGetBallRadiusFunctionPointer(CCObject* obj,GetBallRadiusHandler handler)
{
	getBallRadiusFunction = handler;
	listenerObj = obj;
}