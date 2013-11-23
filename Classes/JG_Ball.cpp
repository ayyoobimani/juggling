#include "JG_Ball.h"
#define COCOS2D_DEBUG 1

float JG_Ball::minSpeed;
float JG_Ball::maxSpeed;

int JG_Ball::ballScoreByLevel[MAX_BALL_LEVELS];

CCString JG_Ball::ballTextureNamesByLevel[MAX_BALL_LEVELS];

CCTexture2D* JG_Ball::ballTexturesByLevel[MAX_BALL_LEVELS];

JG_Ball::JG_Ball(void)
{
	ballTexture = "ball.png";
	tracePointTexture = CCTextureCache::sharedTextureCache()->addImage("TraceDot.png");

	moveMode = EMove_Curve;
	curve_Rad = 0;
	currentSpeed = 0;
	//tempInitialPosition = ccp(50,50);
	
	// give a simple rotation to ball
	action_Rotate = CCRepeatForever::create(CCRotateBy::create(1,360));
	runAction(action_Rotate);

	bDrawThrowPath = false;
	ballThrowDirection = EThrowDirection::EDir_RightHandToUp;

	
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
		ball->setTexture(ballTexturesByLevel[ball->ballLevel]);
		ball->mainGame = game;

		/********** temporary store the initial state for tempReset function *********/
		ball->tempInitialThrowDirection = initialDirection;
		ball->tempInitialPosition = initialPos;
		/*****************************************************************************/
		ball->setScale(GAME_SCALE * BALL_SCALE);
		
		// activating update function 
		//ball->schedule(schedule_selector(JG_Ball::update));
		ball->scheduleUpdate();

		return ball;
	}
	CC_SAFE_DELETE(ball);
	return NULL;
}

void JG_Ball::Throw(float force, CCPoint destination)
{
	// Note: the order is important 
	ballThrowDirection = GetNewThrowDirection(ballThrowDirection);
	moveMode = GetNewMoveMode(moveMode);
	currentSpeed = GetNewSpeedByForce(force);

	//CCLOG("Throw force is %f",force);

	//CCLog(" Throw",0);

	MoveDirX = (destination.x-getPositionX())/abs(destination.x-getPositionX()) ;

	if(moveMode == EMove_Curve)
	{
		curve_Rad = CalculateCurveRad(currentSpeed,this->getPosition(),destination);
			
	}

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
		//if(abs(CC_RADIANS_TO_DEGREES(curve_Rad))<45)
			//curve_Rad = (curve_Rad/abs(curve_Rad)) *CC_DEGREES_TO_RADIANS(90)- curve_Rad;
		//TODO : Do it for reverse direction
		if( CC_RADIANS_TO_DEGREES(tempCurveRad)>-45)
			tempCurveRad = CC_DEGREES_TO_RADIANS(-90) - tempCurveRad;

	
		/* because asinf returns a radian in portion 1 and 4, we convert the portion 4 radian to portaion 2 (between 90 and 180 )*/
		if(tempCurveRad<0)
			tempCurveRad = CC_DEGREES_TO_RADIANS(180) + tempCurveRad;

		//CCLog(" curve rad is %f",CC_RADIANS_TO_DEGREES(tempCurveRad));

		return tempCurveRad;
		//curve_Rad = CC_DEGREES_TO_RADIANS(135);
		//mainGame->gameHUD->debugLabel->setString("");
		//mainGame->gameHUD->debugLabel->setString(CCString::createWithFormat("RAD: %f", CC_RADIANS_TO_DEGREES(curve_Rad))->getCString());
}

float JG_Ball::GetNewSpeedByForce(float force)
{
	
	force -= mainGame->GetActualMinPower();
	//CCLOG("GetNewSpeedByForce actual min is  is %f", (MIN_TOUCH_LENGTH_FACTOR) * mainGame->GetMaxThrowPower());
	//mainGame->gameHUD->debugLabel->setString("");
	//mainGame->gameHUD->debugLabel->setString(CCString::createWithFormat("RAD: %f",force)->getCString());	

	if(moveMode == EMove_Straight)
		return minSpeed ;
	else 
		return clampf( minSpeed + minSpeed * force,minSpeed,maxSpeed);

	// for test on andorid
	//CCLOG("min speed is %f" , minSpeed);
	//CCLog("speed is %f", currentSpeed);

}

EThrowDirection JG_Ball::GetNewThrowDirection(EThrowDirection preDir)
{
	if (preDir==EDir_LeftHandToRight)
		return EDir_RightHandToUp;
	else if(preDir==EDir_RighHandtToLeft)
		return EDir_LeftHandToUp;
	else if( preDir== EDir_RightHandToUp)
		return EDir_LeftHandToRight;
	else //if ( ballThrowDirection== EDir_LeftHandToUp)
		return EDir_RightHandToUp;

}

EMoveMode JG_Ball::GetNewMoveMode(EMoveMode preMoveMode)
{
	if(preMoveMode==EMove_Straight)
		return  EMove_Curve;
	else //if(moveMode == EMove_Curve)
		return EMove_Straight;
}

void JG_Ball::SetThrowPathInfo(float force,CCPoint originPosition, CCPoint destPosition)
{
	throwPath_Force = force;
	throwPath_OriginPosition = originPosition;
	throwPath_destPosition = destPosition;
	bDrawThrowPath = true;
	
}

void JG_Ball::ResetThrowPathInfo(float dt)
{
	bDrawThrowPath= false;
}

void JG_Ball::DrawBallTexture()
{
	//setTexture(
	//ballTexturesByLevel[ballLevel]->drawAtPoint(ccp(0,0));
}

void JG_Ball::DrawThrowPath()
{
	EMoveMode tempMoveMode = moveMode;
	moveMode = EMove_Curve;
	//CCLog("throw Force %f", throwPath_Force);
	float tempSpeed = GetNewSpeedByForce(throwPath_Force);
	//CCLog("tempForce %f" , throwPath_Force);
	//CCLog("tempSpeed %f" , tempSpeed);
	float tempSpeedX,tempSpeedY;
	CCPoint tracePoint;

	float tempCurveRad;
	tempCurveRad = CalculateCurveRad(tempSpeed,throwPath_OriginPosition,throwPath_destPosition);
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
		tracePointTexture->drawAtPoint(convertToNodeSpace(tracePoint));
		//tracePointTexture->SetOr
	}
	bDrawThrowPath = false;
	moveMode = tempMoveMode;
	//this->scheduleOnce(schedule_selector(JG_Ball::ResetThrowPathInfo),BALL_PATH_TRACE_FADE_DELAY);
}

void JG_Ball::draw()
{
	CCSprite::draw();
	DrawBallTexture();
	if(bDrawThrowPath)
		DrawThrowPath();
}


void JG_Ball::update(float dt)
{
	

	//TODO: clean up the code
	//TODO: check performance
	//CCLog("update " ,0);
	if (moveMode==EMove_Curve)
	{


		tempBallSpeedX = currentSpeed * cos(curve_Rad);
		tempBallSpeedY = currentSpeed * sin(curve_Rad);

		// calculate new speeds
		tempBallSpeedX = tempBallSpeedX;
		tempBallSpeedY = -GRAVITY* dt + tempBallSpeedY;

		// calculat new positions based on new speeds
		tempBallNewX = tempBallSpeedX * dt + getPositionX();
		tempBallNewY = tempBallSpeedY * dt + getPositionY();

		// calculate the total speed based on new speeds on each direction
		currentSpeed = sqrt(pow(tempBallSpeedY,2)+pow(tempBallSpeedX,2));

		// calculate new curve_Rad (falling radian) base on new speeds on each direction
		//**************** TODO: find a better soloution *************/
		if(tempBallSpeedX >= 0)
			curve_Rad = atan(tempBallSpeedY/tempBallSpeedX);
		else
			curve_Rad =CC_DEGREES_TO_RADIANS(180)+ atan(tempBallSpeedY/tempBallSpeedX);
		/**************************************************************/	

		setPosition(ccp(tempBallNewX,tempBallNewY));
	}
	else if(moveMode==EMove_Straight)
	{
		//just changing X based on speed and direction 
		setPosition(ccp(getPositionX()+ MoveDirX *(currentSpeed* dt),getPositionY()));
	}
	else
	{
		// not possible
	}


	// temporary for reseting ball
	if( getPositionY() < -20 || getPositionX() < -20 || getPositionX() > mainGame->screenSize.width + 20)
	{
		OutOfScreen();
	}
   
}

void JG_Ball::OutOfScreen()
{

	CCPoint wtf = this->getPosition();
	mainGame->BallLost(this);
}

//
//void JG_Ball::TempReset()
//{
//	//CCLog("Temp reset");
//	setPosition(tempInitialPosition);
//	
//	moveMode = EMove_Curve;
//	curve_Rad = 0;
//	ballThrowDirection = tempInitialThrowDirection;
//	curve_Rad = CC_DEGREES_TO_RADIANS(0); 
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
float JG_Ball::TaylorFormulaCalculate(float angle)
{
	float ffs=angle + 0.5 * pow(angle,3)/3 + 3/8 * pow(angle,5)/5 + 15/ 48 * pow(angle,7)/7 ;;
	//mainGame->gameHUD->debugLabel->setString(CCString::createWithFormat("%f,%f",angle,ffs)->getCString());
	return ffs;
}

float JG_Ball::GetMaxSpeed()
{
	return maxSpeed;
}
float JG_Ball::GetMinSpeed()
{
	return minSpeed;
}

void JG_Ball::SetBallLevel(int newLevel)
{
	ballLevel = newLevel;
	setTexture(ballTexturesByLevel[ballLevel]);
}

int JG_Ball::GetBallScore()
{
	return ballScoreByLevel[ballLevel];
}

float JG_Ball::getCurrentSpeedX()
{
	return tempBallSpeedX;
}

float JG_Ball::getCurrentSpeedY()
{
	return tempBallSpeedY;
}