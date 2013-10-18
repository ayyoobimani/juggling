#include "JG_Ball.h"


JG_Ball::JG_Ball(void)
{
	speed = 20;

	// for let the ball fall
	moveMode = EMove_Curve;
}


JG_Ball::~JG_Ball(void)
{
}

JG_Ball* JG_Ball::createWithFrameName(const char * pszSpriteFrameName) {        JG_Ball * ball = new JG_Ball();	if (ball && ball->initWithSpriteFrameName(pszSpriteFrameName)) {		ball->autorelease();		return ball;	}	CC_SAFE_DELETE(ball);	return NULL;}

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
	curve_Deg = asinf(x * GRAVITY / pow(speed,2) )/2;
	if (abs(curve_Deg)>90 )
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
		X1 = speed* cosf(curve_Deg) *curve_TotalTime +curve_X0 ;
		Y1 = -(GRAVITY * pow(curve_TotalTime,2))/2 + speed * sinf (curve_Deg) +curve_Y0 ;
		setPosition(ccp(X1,Y1));
	}
	else if(moveMode==EMove_Straight)
	{
		setPosition(ccp(getPositionX()*(speed* dt*,getPositionY()));
	}
	else
	{
		// not possible
	}
   
}