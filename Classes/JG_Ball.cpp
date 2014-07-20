#include "JG_Ball.h"
#define COCOS2D_DEBUG 1

float JG_Ball::minSpeed;
float JG_Ball::maxSpeed;


JG_Ball::JG_Ball(void)
{
	radius=(CCDirector::sharedDirector()->getWinSize().height* 20/320) ;
	ballTexture = "ball2.png";
	//setTexture(CCTextureCache::sharedTextureCache()->addImage("deadStar.png"));
	//tracePointTexture = CCTextureCache::sharedTextureCache()->addImage("deadStar.png");

	moveMode = Move_Curve;
	curveRadian = 0;
	currentSpeed = 0;
	//tempInitialPosition = ccp(50,50);
	
	// give a simple rotation to ball
	//action_Rotate = CCRepeatForever::create(CCRotateBy::create(1,360));
	//runAction(action_Rotate);
}


JG_Ball::~JG_Ball(void)
{
	//CCLOG("Ball DELETE " , 0);
	this->unscheduleUpdate();
}

JG_Ball* JG_Ball::CreateBall(JG_Game_Main* game,CCPoint initialPos, EThrowDirection initialDirection, int initialBallLevel) 
{
    
    JG_Ball * ball = new JG_Ball();
	if (ball && ball->initWithFile(ball->ballTexture.getCString()))
	{
		ball->autorelease();
		ball->setPosition(game->getPosition() + initialPos);
		CCPoint wtf=  ball->getPosition();
		ball->ballThrowDirection = initialDirection;
		ball->ballLevel = initialBallLevel;
		ball->mainGame = game;
		ball->setAnchorPoint(CCPoint(0.5,0.5));
	
		// activating update function 
		//ball->schedule(schedule_selector(JG_Ball::update));
		ball->scheduleUpdate();

		//ball->InitialAnimation();


		return ball;
	}
	CC_SAFE_DELETE(ball);
	return NULL;
}

void JG_Ball::CalculateSpeedBoundriesBaseOnLength(float deltaX)
{
	//TODO: wtf +2 needed
	minSpeed = sqrt( GRAVITY *JG_abs(deltaX))+ 2;
	//TODO: calculate maxSpeed
	maxSpeed = sqrt(GRAVITY *JG_abs(deltaX)/sin(2 *MAX_THROW_RAD));
}


void JG_Ball::Throw(float force, CCPoint destination)
{

	mainGame->OnBallThrow(this);
	// Note: the order is important 
	ballThrowDirection = GetNewThrowDirection(ballThrowDirection);
	moveMode = GetNewMoveMode(moveMode);
	currentSpeed = GetNewSpeedByForce(force);

	MoveDirX = (destination.x-getPositionX())/JG_abs(destination.x-getPositionX()) ;

	if(moveMode == Move_Curve)
		curveRadian = CalculateCurveRad(currentSpeed,this->getPosition(),destination);
}

float JG_Ball::CalculateCurveRad(float speed,CCPoint originPosition, CCPoint destPosition)
{
	float tempCurveRad;
	//tempCurveRad =asinf((destPosition.x-getPositionX()) * GRAVITY / pow(speed,2))/2.0;
	tempCurveRad = asin((destPosition.x-originPosition.x) * GRAVITY / pow(speed,2))/2;
		
		
		

	/* because there are two radians the have the same range (they can both reach the 
		destination in same time), we choose the bigger one for better curve.
		we can throw the ball with 30 deg and 60 deg and they will reach the destination
		but we choose the 60 deg.
		*/
	//if(JG_abs(CC_RADIANS_TO_DEGREES(curveRadian))<45)
		//curveRadian = (curveRadian/JG_abs(curveRadian)) *CC_DEGREES_TO_RADIANS(90)- curveRadian;
	//TODO : Do it for reverse direction
	if( CC_RADIANS_TO_DEGREES(tempCurveRad)>-45)
		tempCurveRad = CC_DEGREES_TO_RADIANS(-90) - tempCurveRad;

	
	/* because asinf returns a radian in portion 1 and 4, we convert the portion 4 radian to portaion 2 (between 90 and 180 )*/
	if(tempCurveRad<0)
		tempCurveRad = CC_DEGREES_TO_RADIANS(180) + tempCurveRad;

	//CCLog(" curve rad is %f",CC_RADIANS_TO_DEGREES(tempCurveRad));

	return tempCurveRad;
}

float JG_Ball::GetNewSpeedByForce(float force)
{

	if(moveMode == Move_Straight)
		return minSpeed ;
	else 
		return clampf( minSpeed + minSpeed * force,minSpeed,maxSpeed);
}

EThrowDirection JG_Ball::GetNewThrowDirection(EThrowDirection preDir)
{
	//TODO: convert it into switch case
	if (preDir==Dir_LeftHandToRight)
		return Dir_RightHandToUp;
	else if(preDir==Dir_RighHandtToLeft)
		return Dir_LeftHandToUp;
	else if( preDir== Dir_RightHandToUp)
		return Dir_LeftHandToRight;
	else //if ( ballThrowDirection== EDir_LeftHandToUp)
		return Dir_RightHandToUp;

}

EMoveMode JG_Ball::GetNewMoveMode(EMoveMode preMoveMode)
{
	if(preMoveMode==Move_Straight)
		return  Move_Curve;
	else //if(moveMode == EMove_Curve)
		return Move_Straight;
}


void JG_Ball::SetThrowPathInfo(float force,CCPoint originPosition, CCPoint destPosition)
{
	throwPath_Force = force;
	throwPath_OriginPosition = originPosition;
	throwPath_DestPosition = destPosition;
	bDrawThrowPath = true;
	
}

void JG_Ball::ResetThrowPathInfo(float dt)
{
	bDrawThrowPath= false;
}



void JG_Ball::DrawThrowPath()
{
	EMoveMode tempMoveMode = moveMode;
	moveMode = Move_Curve;
	//CCLog("throw Force %f", throwPath_Force);
	float tempSpeed = GetNewSpeedByForce(throwPath_Force);
	float tempSpeedX,tempSpeedY;
	CCPoint tracePoint;

	float tempCurveRad;
	tempCurveRad = CalculateCurveRad(tempSpeed,throwPath_OriginPosition,throwPath_DestPosition);
	//CCLog("tempCurve Is %f", CC_RADIANS_TO_DEGREES(tempCurveRad));

	tracePoint = throwPath_OriginPosition;
	tempSpeedX= tempSpeed * cos(tempCurveRad);
	tempSpeedY = tempSpeed * sin(tempCurveRad);
	for( int i = 0 ; i< BALL_PATH_TRACE_STEPS ; i++)
	{
		
		tempSpeedY = -GRAVITY* BALL_PATH_TRACE_INTERVALS  + tempSpeedY;
		//tempSpeedX = tempSpeedX;
		tracePoint.x = tempSpeedX * BALL_PATH_TRACE_INTERVALS + tracePoint.x;
		tracePoint.y = tempSpeedY * BALL_PATH_TRACE_INTERVALS + tracePoint.y;
		tracePointTexture->drawAtPoint(convertToNodeSpace(tracePoint-tracePointTexture->getContentSize()/2));
		//tracePointTexture->SetOr
	}
	bDrawThrowPath = false;
	moveMode = tempMoveMode;
}


void JG_Ball::update(float dt)
{
	ProcessMove(dt);
	CheckCollisionWithBall();
	CheckOutOfScreen();
	AdjustRotation(); 
}


//TODO: clean up the code
//TODO: check performance
void JG_Ball::ProcessMove(float dt)
{
	if (moveMode==Move_Curve)
	{


		tempBallSpeedX = currentSpeed * cos(curveRadian);
		tempBallSpeedY = currentSpeed * sin(curveRadian);

		// calculate new speeds
		tempBallSpeedX = tempBallSpeedX;
		tempBallSpeedY = -GRAVITY* dt + tempBallSpeedY;

		// calculat new positions based on new speeds
		tempBallNewX = (tempBallSpeedX * dt + getPositionX());
		tempBallNewY = (tempBallSpeedY * dt + getPositionY());

		// calculate the total speed based on new speeds on each direction
		currentSpeed = sqrt(pow(tempBallSpeedY,2)+pow(tempBallSpeedX,2));

		// calculate new curveRadian (falling radian) base on new speeds on each direction
		//**************** TODO: find a better soloution *************/
		if(tempBallSpeedX >= 0)
			curveRadian = atan(tempBallSpeedY/tempBallSpeedX);
		else
			curveRadian =CC_DEGREES_TO_RADIANS(180)+ atan(tempBallSpeedY/tempBallSpeedX);
		/**************************************************************/	

		setPosition(ccp(tempBallNewX,tempBallNewY));
	}
	else if(moveMode==Move_Straight)
	{
		//just changing X based on speed and direction 
		setPosition(ccp(getPositionX()+ MoveDirX *(currentSpeed* dt),getPositionY()));
	}
	else
	{
		// not possible
	}


}

void JG_Ball::CheckOutOfScreen()
{

	if( getPositionY() < -20 || getPositionX() < -20 || getPositionX() > mainGame->screenSize.width + 20)
	{;
		mainGame->OnBallLost(this);
	}
}

void JG_Ball::AdjustRotation()
{
	setRotation(GetRotationBasedOnDirection());
}

//TODO: realy dude. WTF is this mess
float JG_Ball::GetRotationBasedOnDirection()
{
	if(moveMode==Move_Straight)
	{
		if(getScaleY()<0)
			setScaleY(getScaleY()*-1);

		return (90-90* MoveDirX);
	}
	else
	{
		if(getScaleY()>0)
			setScaleY(getScaleY()*-1);
	
		return -CC_RADIANS_TO_DEGREES(curveRadian);
		
	}	
}
//
//void JG_Ball::TempReset()
//{
//	//CCLog("Temp reset");
//	setPosition(tempInitialPosition);
//	
//	moveMode = EMove_Curve;
//	curveRadian = 0;
//	ballThrowDirection = tempInitialThrowDirection;
//	curveRadian = CC_DEGREES_TO_RADIANS(0); 
//	currentSpeed = 0;
//}


void JG_Ball::SetInitialTouchPosition(CCPoint newTouchPos)
{
	InitialTouchPosition = newTouchPos;
}

CCPoint JG_Ball::GetInitialTouchPosition()
{
	return InitialTouchPosition;
}

EThrowDirection JG_Ball::GetBallDirection()
{
	return ballThrowDirection;
}
//float JG_Ball::TaylorFormulaCalculate(float angle)
//{
//	float ffs=angle + 0.5 * pow(angle,3)/3 + 3/8 * pow(angle,5)/5 + 15/ 48 * pow(angle,7)/7 ;;
//	//mainGame->gameHUD->debugLabel->setString(CCString::createWithFormat("%f,%f",angle,ffs)->getCString());
//	return ffs;
//}

float JG_Ball::GetMaxSpeed()
{
	return maxSpeed;
}
float JG_Ball::GetMinSpeed()
{
	return minSpeed;
}


float JG_Ball::GetCurrentSpeedX()
{
	if(moveMode == Move_Curve)
		return tempBallSpeedX;
	else
		return currentSpeed ;
}
//adding a comment
float JG_Ball::GetCurrentSpeedY()
{
	if(moveMode == Move_Curve)
		return tempBallSpeedY;
	else
		return 0;
}

void JG_Ball::CheckCollisionWithBall()
{
	JG_Ball* tempCurrentBall;
	for(int i=0;i<mainGame->GetBallArray()->count();i++)
	{
		tempCurrentBall=(JG_Ball*)mainGame->GetBallArray()->objectAtIndex(i);
		if(this->GetBallDirection()==tempCurrentBall->GetBallDirection() && this!=tempCurrentBall)
		{
			if(ArePointsColliding(this->getPosition(),(tempCurrentBall)->getPosition(),radius+radius))
				mainGame->OnBallsCollide(tempCurrentBall,this);
		}
		
	}
	

}

void JG_Ball:: SetShineVisibility(bool value)
{
	bMustShine = value;
}


EMoveMode JG_Ball::GetMoveMode()
{
	return moveMode;
}