#pragma once
#include "sprite_nodes\CCSprite.h"
#include "actions\CCActionInterval.h"
#include "cocos2d.h"



using namespace cocos2d;

#define GRAVITY 400

enum EThrowDirection
{
	EDir_LeftHandToRight, EDir_RighHandtToLeft, EDir_LeftHandToUp, EDir_RightHandToUp
};

enum EMoveMode
{
	EMove_Curve, EMove_Straight
};

/*! The main class for handling ball movement */
class JG_Ball :
	public CCSprite
{
public:
	JG_Ball(void);
	virtual ~JG_Ball(void);

	static float minSpeed;
	static float maxSpeed;
	float currentSpeed;
	
	EThrowDirection ballThrowDirection;

	/*! radians of falling in EMove_Curve */
	float curve_Rad;

	/*! direction of movement in EMove_Straight */
	float straight_Dir;

	CCString ballTexture;

	/*! temporary storing Initial state for TempReset function */
	CCPoint tempInitialPosition;
	EThrowDirection tempInitialThrowDirection;

	/*! temporary rotation action */
	CCRepeatForever* action_Rotate;

	/*!  The movement mode of the ball */
	EMoveMode moveMode;

	/*! Stores the touch potsion when ball is touched for first time.
	    This is used for determining the direction of throw (touch) */
	CCPoint InitialTouchPosition;

	/*! Creating ball in a specific position and specific direction */
	static JG_Ball* CreateBall(CCPoint initialPos,EThrowDirection initialDirection);

	/*! Initial the curve movement variables */
	void MoveCurve(float force,CCPoint destination);

	/*! Initial the straight movement variables */
	void MoveStaight(float force, CCPoint destination);

	/*! Handles the movement based on the current mode */
	void update(float dt);

	/*! temporary reset the ball to it's initial postion when it is out of view */
	void TempReset();

	/*! get the direction of ball */
	EThrowDirection GetBallDirection();

	CCPoint GetInitialTouchPosition();
	void SetInitialTouchPosition(CCPoint newTouchPos);

	/*! Calculate the minimum Speed , based on the distance of handes */
	static void CalculateSpeedBoundriesBaseOnLength(float deltaX)
	{
		minSpeed = sqrt( GRAVITY * abs(deltaX));
	}
};





