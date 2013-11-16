#include "JG_Ball.h"
#define COCOS2D_DEBUG 1

float JG_Ball::minSpeed;
float JG_Ball::maxSpeed;

JG_Ball::JG_Ball(void)
{
	ballTexture = "ball.png";

	moveMode = EMove_Curve;
	curve_Rad = 0;
	currentSpeed = 0;
	//tempInitialPosition = ccp(50,50);
	
	// give a simple rotation to ball
	action_Rotate = CCRepeatForever::create(CCRotateBy::create(1,360));
	runAction(action_Rotate);

}


JG_Ball::~JG_Ball(void)
{
	CCLOG("Ball DELETE " , 0);
	this->unscheduleUpdate();
}

JG_Ball* JG_Ball::CreateBall(JG_Game_Main* game,CCPoint initialPos, EThrowDirection initialDirection) 
{
    
    JG_Ball * ball = new JG_Ball();
	if (ball && ball->initWithFile(ball->ballTexture.getCString()))
	{
		ball->autorelease();
		ball->setPosition(game->getPosition() + initialPos);
		CCPoint wtf=  ball->getPosition();
		ball->ballThrowDirection = initialDirection;
		ball->ballScore = 20;
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
	DetermineNewThrowDirection();	
	DetermineNewMoveMode();
	DetermineNewSpeedByForce(force);

	CCLog(" Throw",0);

	MoveDirX = (destination.x-getPositionX())/abs(destination.x-getPositionX()) ;

	if(moveMode == EMove_Curve)
	{
		
		curve_Rad =TaylorFormulaCalculate((destination.x-getPositionX()) * GRAVITY / pow(currentSpeed,2))/2.0;
		//curve_Rad = asin((destination.x-getPositionX()) * GRAVITY / pow(currentSpeed,2))/2;
		
		//mainGame->gameHUD->debugLabel->setString("");
		//mainGame->gameHUD->debugLabel->setString(CCString::createWithFormat("RAD: %f",force)->getCString());
		

		/* because there are two radians the have the same range (they can both reach the 
			destination in same time), we choose the bigger one for better curve.
			we can throw the ball with 30 deg and 60 deg and they will reach the destination
			but we choose the 60 deg.
			*/
		//if(abs(CC_RADIANS_TO_DEGREES(curve_Rad))<45)
			//curve_Rad = (curve_Rad/abs(curve_Rad)) *CC_DEGREES_TO_RADIANS(90)- curve_Rad;
		//TODO : Do it for reverse direction
		if( CC_RADIANS_TO_DEGREES(curve_Rad)>-45)
			curve_Rad = CC_DEGREES_TO_RADIANS(-90) - curve_Rad;

	
		/* because asinf returns a radian in portion 1 and 4, we convert the portion 4 radian to portaion 2 (between 90 and 180 )*/
		if(curve_Rad<0)
			curve_Rad = CC_DEGREES_TO_RADIANS(180) + curve_Rad;

		CCLog(" curve rad is %f",CC_RADIANS_TO_DEGREES(curve_Rad));

		//curve_Rad = CC_DEGREES_TO_RADIANS(135);
		//mainGame->gameHUD->debugLabel->setString("");
		//mainGame->gameHUD->debugLabel->setString(CCString::createWithFormat("RAD: %f", CC_RADIANS_TO_DEGREES(curve_Rad))->getCString());
	}

	


}

void JG_Ball::DetermineNewSpeedByForce(float force)
{
	currentSpeed = clampf(minSpeed/2 + minSpeed * force,minSpeed,maxSpeed);
	if(moveMode == EMove_Straight)
		currentSpeed = minSpeed ;
	// for test on andorid
	if(currentSpeed> maxSpeed)
		currentSpeed = maxSpeed;
	CCLOG("min speed is %f" , minSpeed);
	CCLog("speed is %f", currentSpeed);

}

void JG_Ball::DetermineNewThrowDirection()
{
	if (ballThrowDirection==EDir_LeftHandToRight)
		ballThrowDirection=EDir_RightHandToUp;
	else if(ballThrowDirection==EDir_RighHandtToLeft)
		ballThrowDirection=EDir_LeftHandToUp;
	else if( ballThrowDirection== EDir_RightHandToUp)
		ballThrowDirection=EDir_LeftHandToRight;
	else //if ( ballThrowDirection== EDir_LeftHandToUp)
		ballThrowDirection= EDir_RightHandToUp;

}

void JG_Ball::DetermineNewMoveMode()
{
	if(moveMode==EMove_Straight)
		moveMode = EMove_Curve;
	else //if(moveMode == EMove_Curve)
		moveMode = EMove_Straight;
}


void JG_Ball::update(float dt)
{
	

	//TODO: clean up the code
	//TODO: check performance
	//CCLog("update " ,0);
	if (moveMode==EMove_Curve)
	{


		tempSpeedX = currentSpeed * cos(curve_Rad);
		tempSpeedY = currentSpeed * sin(curve_Rad);

		// calculate new speeds
		tempSpeedX = tempSpeedX;
		tempSpeedY = -GRAVITY* dt + tempSpeedY;

		// calculat new positions based on new speeds
		tempNewX = tempSpeedX * dt + getPositionX();
		tempNewY = tempSpeedY * dt + getPositionY();

		// calculate the total speed based on new speeds on each direction
		currentSpeed = sqrt(pow(tempSpeedY,2)+pow(tempSpeedX,2));

		// calculate new curve_Rad (falling radian) base on new speeds on each direction
		//**************** TODO: find a better soloution *************/
		if(tempSpeedX >= 0)
			curve_Rad = atan(tempSpeedY/tempSpeedX);
		else
			curve_Rad =CC_DEGREES_TO_RADIANS(180)+ atan(tempSpeedY/tempSpeedX);
		/**************************************************************/	

		setPosition(ccp(tempNewX,tempNewY));
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

