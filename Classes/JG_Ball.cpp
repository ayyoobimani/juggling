#include "JG_Ball.h"
#define COCOS2D_DEBUG 1

float JG_Ball::minSpeed;

JG_Ball::JG_Ball(void)
{
	tempDraw = NULL;


	// for let the ball fall
	moveMode = EMove_Curve;

	tempDraw = JG_TempLineContainer::create();
//	this->addChild(tempDraw);
	//CCDirector::sharedDirector()->getRunningScene()->addChild(tempDraw);
	tempDraw->retain();
	

	tempDraw->setPosition(0,0);

	this->schedule(schedule_selector(JG_Ball::update));
}


JG_Ball::~JG_Ball(void)
{
}

JG_Ball* JG_Ball::createWithFileName(const char * pszFileName,CCPoint initialPos) 
{
    
    JG_Ball * ball = new JG_Ball();
	if (ball && ball->initWithFile(pszFileName))
	{
		ball->autorelease();
		ball->setPosition(initialPos);
		ball->curve_X0 = ball->getPositionX();
		ball->curve_Y0 = ball->getPositionY();
		ball->curve_Rad = CC_DEGREES_TO_RADIANS(-90); 
		ball->curve_TotalTime = 0;
		ball->speed = 0;
		return ball;
	}
	CC_SAFE_DELETE(ball);
	return NULL;
}


void JG_Ball::MoveStaight(float force, CCPoint destination)
{
	moveMode = EMove_Straight;
	speed = minSpeed;
	straight_Dir = (destination.x-getPositionX())/abs(destination.x-getPositionX()) ;
}

void JG_Ball::MoveCurve(float force,CCPoint destinaion)
{
	
	moveMode=EMove_Curve;
	speed = minSpeed;
	
	curve_Rad = asinf((destinaion.x-getPositionX()) * GRAVITY / pow(speed,2))/2;

	float temp = CC_RADIANS_TO_DEGREES(curve_Rad);

	if(abs(CC_RADIANS_TO_DEGREES(curve_Rad))<45)
		curve_Rad = (curve_Rad/abs(curve_Rad)) *CC_DEGREES_TO_RADIANS(90)- curve_Rad;
	
	if(curve_Rad<0)
		curve_Rad = CC_DEGREES_TO_RADIANS(180) + curve_Rad;
	CCLog(" curve rad is %f",CC_RADIANS_TO_DEGREES(curve_Rad));
	// bayad begim age sor@ kamtar azin bashe che etefaghi biyofte :| 


}

void JG_Ball::update(float dt)
{
	

	//TODO: clean up the code
	//TODO: check performance
	
	if (moveMode==EMove_Curve)
	{
		float newX,newY;
		float speedY,speedX;
		speedX = speed * cosf(curve_Rad);
		speedY = speed * sinf(curve_Rad);
		
		
		//CCLOG("deg is %f ",cosf(curve_Rad) ) ;
		speedX = speedX;
		speedY = -GRAVITY* dt + speedY;

		newX = speedX * dt + getPositionX();
		newY = speedY * dt + getPositionY();
		speed = sqrt(pow(speedY,2)+pow(speedX,2));

		float deg1 = CC_RADIANS_TO_DEGREES(curve_Rad) ;
		curve_Rad =CC_DEGREES_TO_RADIANS(180* speedY/abs(speedY))+ atan(speedY/speedX);
		float deg2 = CC_RADIANS_TO_DEGREES(curve_Rad) ;
		//CCLOG("speedY %f",speed);
		
		//Y1  =getPositionY();
		//X1 = getPositionX();
		setPosition(ccp(newX,newY));
		CCDrawNode ccnode;
		ccColor4F color;
		color.b = 255;
		if(tempDraw)
			tempDraw->setPosition(newX,newY);
	}
	else if(moveMode==EMove_Straight)
	{
		setPosition(ccp(getPositionX()+ straight_Dir *(speed* dt),getPositionY()));
	}
	else
	{
		// not possible
	}


	if( getPositionY() < -20 || getPositionX() < -20 || getPositionX() > CCDirector::sharedDirector()->getWinSize().width + 20)
	{
		tempReset();
	}
   
}

void JG_Ball::tempReset()
{
	setPosition(ccp(40,200));

	moveMode = EMove_Curve;
	curve_Rad = 0;
	curve_Rad = CC_DEGREES_TO_RADIANS(-90); 
	//TODO: why speed 0 is not working
	speed = 10;


}


void JG_Ball::SetInitialTouchPosition(CCPoint newTouchPos)
{
	touchPosition = newTouchPos;
}

CCPoint JG_Ball::GetInitialTouchPosition()
{
	return touchPosition;
}