#pragma once
#include "sprite_nodes\CCSprite.h"
#include "actions\CCActionInterval.h"
#include "JG_TempLineContainer.h"
#include "cocos2d.h"


using namespace cocos2d;

#define GRAVITY 300




enum EMoveMode
{
	EMove_Curve, EMove_Straight
};

class JG_Ball :
	public CCSprite
{
public:
	JG_Ball(void);
	virtual ~JG_Ball(void);

	static float minSpeed;
	static float maxSpeed;
	float speed;

	float curve_TotalTime ;
	float curve_Y0,curve_X0;
	float curve_Rad;
	float straight_Dir;

	JG_TempLineContainer * tempDraw;
	CCPoint touchPosition;

	// determine the movement mode of the ball
	EMoveMode moveMode;

	//TODO: change the name
	static JG_Ball* createWithFileName(const char * pszFileName,CCPoint initialPos);

	void MoveCurve(float force,CCPoint destination);
	void MoveStaight(float force, CCPoint destination);
	void update(float dt);
	void tempReset();


	CCPoint GetInitialTouchPosition();

	void SetInitialTouchPosition(CCPoint newTouchPos);
	//TODO: implement this
	static void CalculateSpeedBoundriesBaseOnLength(float x)
	{
		minSpeed = 400;
	}
};





