#pragma once
#include "sprite_nodes\CCSprite.h"
#include "actions\CCActionInterval.h"
#include "cocos2d.h"


using namespace cocos2d;

#define GRAVITY 20

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
	float curve_Deg;
	float straight_Dir;

	CCPoint touchPosition;

	MoveMode moveMode;

	static JG_Ball* createWithFrameName(const char * pszSpriteFrameName);

	void MoveCurve(float force,float x, float y);
	void MoveStaight(float force, float x , float y);
	void update(float dt);

	//TODO: move them to .cpp
	CCPoint GetTochPosition()
	{
		return touchPosition;
	}
	void SetTouchPosition(CCPoint newTouchPos)
	{
		touchPosition = newTouchPos;
	}
};

