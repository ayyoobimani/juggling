#include "JG_Ball.h"
#define COCOS2D_DEBUG 1


float JG_Ball::minSpeed;

JG_Ball::JG_Ball(void)
{
	ballTexture = "ball.png";

	moveMode = EMove_Curve;
	curve_Rad = 0;
	
	// give a simple rotation to ball
	action_Rotate = CCRepeatForever::create(CCRotateBy::create(1,360));
	runAction(action_Rotate);

	// activating update function 
	this->schedule(schedule_selector(JG_Ball::update));
}


JG_Ball::~JG_Ball(void)
{
}

JG_Ball* JG_Ball::CreateBall(CCPoint initialPos, EThrowDirection initialDirection) 
{
    
    JG_Ball * ball = new JG_Ball();
	if (ball && ball->initWithFile(ball->ballTexture.getCString()))
	{
		ball->autorelease();
		ball->setPosition(initialPos);
		ball->ballThrowDirection = initialDirection;
		ball->ballScore = 20;

		/********** temporary store the initial state for tempReset function *********/
		ball->tempInitialThrowDirection = initialDirection;
		ball->tempInitialPosition = initialPos;
		/*****************************************************************************/

		return ball;
	}
	CC_SAFE_DELETE(ball);
	return NULL;
}


void JG_Ball::MoveStaight(float force, CCPoint destination)
{
	moveMode = EMove_Straight;
	currentSpeed = minSpeed;

	straight_Dir = (destination.x-getPositionX())/abs(destination.x-getPositionX()) ;

	if(straight_Dir>0)
		ballThrowDirection = EDir_LeftHandToRight;
	else
		ballThrowDirection = EDir_RighHandtToLeft;
}

void JG_Ball::MoveCurve(float force,CCPoint destinaion)
{
	
	moveMode=EMove_Curve;

	//determine direction of the ball
	if (ballThrowDirection==EDir_LeftHandToRight)
		ballThrowDirection=EDir_RightHandToUp;
	else if(ballThrowDirection==EDir_RighHandtToLeft)
		ballThrowDirection=EDir_LeftHandToUp;
	
	currentSpeed = minSpeed + minSpeed *CCRANDOM_0_1()/2   ;
	
	
	curve_Rad = asinf((destinaion.x-getPositionX()) * GRAVITY / pow(currentSpeed,2))/2;

	/* because there are two radians the have the same range (they can both reach the 
		destination in same time), we choose the bigger one for better curve.
		we can throw the ball with 30 deg and 60 deg and they will reach the destination
		but we choose the 60 deg.
		*/
	if(abs(CC_RADIANS_TO_DEGREES(curve_Rad))<45)
		curve_Rad = (curve_Rad/abs(curve_Rad)) *CC_DEGREES_TO_RADIANS(90)- curve_Rad;
	
	/* because asinf returns a radian in portion 1 and 4, we convert the portion 4 radian to portaion 2 (between 90 and 180 )*/
	if(curve_Rad<0)
		curve_Rad = CC_DEGREES_TO_RADIANS(180) + curve_Rad;
	CCLog(" curve rad is %f",CC_RADIANS_TO_DEGREES(curve_Rad));

}

void JG_Ball::update(float dt)
{
	

	//TODO: clean up the code
	//TODO: check performance
	
	if (moveMode==EMove_Curve)
	{
		float newX,newY;
		float speedY,speedX;

		speedX = currentSpeed * cosf(curve_Rad);
		speedY = currentSpeed * sinf(curve_Rad);

		// calculate new speeds
		speedX = speedX;
		speedY = -GRAVITY* dt + speedY;

		// calculat new positions based on new speeds
		newX = speedX * dt + getPositionX();
		newY = speedY * dt + getPositionY();

		// calculate the total speed based on new speeds on each direction
		currentSpeed = sqrt(pow(speedY,2)+pow(speedX,2));

		// calculate new curve_Rad (falling radian) base on new speeds on each direction
		//**************** TODO: find a better soloution *************/
		if(speedX >= 0)
			curve_Rad = atan(speedY/speedX);
		else
			curve_Rad =CC_DEGREES_TO_RADIANS(180)+ atan(speedY/speedX);
		/**************************************************************/	

		setPosition(ccp(newX,newY));
	}
	else if(moveMode==EMove_Straight)
	{
		//just changing X based on speed and direction 
		setPosition(ccp(getPositionX()+ straight_Dir *(currentSpeed* dt),getPositionY()));
	}
	else
	{
		// not possible
	}


	// temporary for reseting ball
	if( getPositionY() < -20 || getPositionX() < -20 || getPositionX() > CCDirector::sharedDirector()->getWinSize().width + 20)
	{
		OutOfScreen();
	}
   
}

void JG_Ball::OutOfScreen()
{

	TempReset();
}
void JG_Ball::TempReset()
{
	setPosition(tempInitialPosition);
	CCLog("Temp reset");
	moveMode = EMove_Curve;
	curve_Rad = 0;
	ballThrowDirection = tempInitialThrowDirection;
	curve_Rad = CC_DEGREES_TO_RADIANS(0); 
	//TODO: why speed 0 is not working
	currentSpeed = 0;


}


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