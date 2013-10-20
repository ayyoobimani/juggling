#pragma once
#include "sprite_nodes\CCSprite.h"
#include "actions\CCActionInterval.h"
#include "JG_TempLineContainer.h"
#include "cocos2d.h"



using namespace cocos2d;

#define GRAVITY 400

enum EDirection
{
	EDir_Left, EDir_Right, EDir_Up, EDir_Down
};

enum EMoveMode
{
	EMove_Curve, EMove_Straight
};
/*! \A class for handling ball */
class JG_Ball :
	public CCSprite
{
public:
	JG_Ball(void);
	virtual ~JG_Ball(void);

	static float minSpeed;
	static float maxSpeed;
	float speed;
	
	EDirection ballDirection;

	float curve_TotalTime ;
	float curve_Y0,curve_X0;
	float curve_Rad;
	float straight_Dir;

	CCPoint tempInitialPosition;

	CCRepeatForever* action_Rotate;

	JG_TempLineContainer * tempDraw;
	CCPoint touchPosition;

	// determine the movement mode of the ball
	EMoveMode moveMode;

	//TODO: change the name
	/*! Creating ball in a specific position */
	static JG_Ball* createWithFileName(const char * pszFileName,CCPoint initialPos);
	/*! Initial the curve movement variables */
	void MoveCurve(float force,CCPoint destination);
	/*! Initial the straight movement variables */
	void MoveStaight(float force, CCPoint destination);
	/*! Handles the movement based on the current mode */
	void update(float dt);
	void tempReset();
	/*! get the direction of ball */
	EDirection GetBallDirection()
	{
		return ballDirection;
	}


	CCPoint GetInitialTouchPosition();

	void SetInitialTouchPosition(CCPoint newTouchPos);
	//TODO: implement this
	/*! Calculate the minimum Speed , based on the distance of handes */
	static void CalculateSpeedBoundriesBaseOnLength(float deltaX)
	{
		// there is a tolerance for now
		minSpeed = sqrt( GRAVITY * abs(deltaX));
	}
};





