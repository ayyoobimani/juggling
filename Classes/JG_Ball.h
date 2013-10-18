#pragma once
#include "sprite_nodes\CCSprite.h"
#include "actions\CCActionInterval.h"
#include "cocos2d.h"


using namespace cocos2d;

#define GRAVITY -100



enum MoveMode
{
	EMove_Curve, EMove_Straight
};

class JG_Ball :
	public CCSprite
{
public:
	JG_Ball(void);
	virtual ~JG_Ball(void);

	float speed;

	float curve_TotalTime ;
	float curve_Y0,curve_X0;
	float curve_Rad;
	float straight_Dir;

	CCPoint touchPosition;

	// determine the movement mode of the ball
	MoveMode moveMode;

	static JG_Ball* createWithFileName(const char * pszFileName,CCPoint initialPos);

	void MoveCurve(float force,float x, float y);
	void MoveStaight(float force, float x , float y);
	void update(float dt);

	//TODO: move them to .cpp
	CCPoint GetInitialTouchPosition()
	{
		return touchPosition;
	}
	void SetInitialTouchPosition(CCPoint newTouchPos)
	{
		touchPosition = newTouchPos;
	}
};

