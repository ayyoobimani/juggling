#include "JG_Ball.h"
#define COCOS2D_DEBUG 1

JG_Ball::JG_Ball(void)
{
	speed = 100;

	// for let the ball fall
	moveMode = EMove_Curve;


	this->schedule(schedule_selector(JG_Ball::update));
}


JG_Ball::~JG_Ball(void)
{
}

JG_Ball* JG_Ball::createWithFileName(const char * pszFileName,CCPoint initialPos) {        JG_Ball * ball = new JG_Ball();	if (ball && ball->initWithFile(pszFileName)) {		ball->autorelease();		ball->setPosition(initialPos);		ball->curve_X0 = ball->getPositionX();		ball->curve_Y0 = ball->getPositionY();		ball->curve_Rad = CC_DEGREES_TO_RADIANS(135); 		ball->curve_TotalTime = 0;		return ball;	}	CC_SAFE_DELETE(ball);	return NULL;}

void JG_Ball::MoveStaight(float force, float x, float y)
{
	moveMode = EMove_Straight;
	straight_Dir = (x-getPositionX())/abs(x-getPositionX()) ;
}

void JG_Ball::MoveCurve(float force, float x ,float y)
{
	
	moveMode=EMove_Curve;
	curve_TotalTime=0;
	curve_X0 = getPositionX();
	curve_Y0 = getPositionY();
	curve_Rad = asinf(x * GRAVITY / pow(speed,2) )/2;
	if (abs(curve_Rad)>90 )
	{
		/********** for getting an NULL error ******/
		JG_Ball * ball;
		ball->getActionByTag(2);
		/*******************************************/
		return ;
	}
	// bayad begim age sor@ kamtar azin bashe che etefaghi biyofte :| 


}

void JG_Ball::update(float dt)
{
	
	
	if (moveMode==EMove_Curve)
	{
		float X1,Y1 ;
		curve_TotalTime+=dt;
		float sin = sinf(curve_Rad);
		X1 = speed * dt  * cosf(curve_Rad) +getPositionX() ;
		//CCLOG("deg is %f ",cosf(curve_Rad) ) ;
		Y1 = -(GRAVITY * pow(dt,2))/2 + speed*dt * sinf (curve_Rad) +getPositionY() ;
		//Y1  =getPositionY();
		setPosition(ccp(X1,Y1));
	}
	else if(moveMode==EMove_Straight)
	{
		setPosition(ccp(getPositionX()+ straight_Dir *(speed* dt),getPositionY()));
	}
	else
	{
		// not possible
	}
   
}