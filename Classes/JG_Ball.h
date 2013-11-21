#pragma once
#include "sprite_nodes\CCSprite.h"
#include "JG_Game_Main.h"
#include "actions\CCActionInterval.h"
#include "cocos2d.h"

#define ARCSIN(__ANGLE__) (__ANGLE__ + 0.5 * pow(__ANGLE__,3)/3 + 3/8 * pow(__ANGLE__,5)/5 + 15/ 48 * pow(__ANGLE__,7)/7)

using namespace cocos2d;

class JG_Game_Main;

#define GRAVITY CCDirector::sharedDirector()->getWinSize().height * 0.6
#define MAX_THROW_RAD (CC_DEGREES_TO_RADIANS(80))
#define BALL_SCALE 1.0

// Step counts for tracking Path of the Ball
#define BALL_PATH_TRACE_STEPS 20
// Interval time for tracking Path of the Ball
#define BALL_PATH_TRACE_INTERVALS 0.05

#define BALL_PATH_TRACE_FADE_DELAY 1

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
	int ballScore;

	JG_Game_Main* mainGame;

	/******************* temporary variables ****************/
	/*this are the variables that are used in high frequency functions
		instead of creating them each time, here we to it once (for performance */

	float tempBallNewX,tempBallNewY;
	float tempBallSpeedY,tempBallSpeedX;
	/****************** /temporary variables ****************/

	
	CCTexture2D* tracePointTexture;
	bool bDrawThrowPath;

	float throwPath_Force;
	CCPoint throwPath_OriginPosition, throwPath_destPosition;

public:
	JG_Ball(void);
	virtual ~JG_Ball(void);

	int GetBallScore(){return ballScore;}

	void OutOfScreen();


	static float minSpeed;
	static float maxSpeed;
	float currentSpeed;
	
	EThrowDirection ballThrowDirection;

	/*! radians of falling in EMove_Curve */
	float curve_Rad;
	/*! movement direction in X (-1 or 1) */
	float MoveDirX;


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
	static JG_Ball* CreateBall(JG_Game_Main* game,CCPoint initialPos,EThrowDirection initialDirection);

	/*! Initial Throw movement variables */
	void Throw(float force,CCPoint destination);


	/*! Handles the movement based on the current mode */
	void update(float dt);

	float GetNewSpeedByForce(float force);
	/*! Returns the new ThrowDirection based on the given previous direction */
	EThrowDirection GetNewThrowDirection(EThrowDirection preDir);
	/*! Returns the new MoveMove based on the given previous ModeMode */
	EMoveMode GetNewMoveMode(EMoveMode preMoveMode);

	/*! Calculate Curve Radian By the given Speed, Origin Position and destination position */
	float CalculateCurveRad(float speed,CCPoint originPosition,CCPoint destPosition);

	void draw();

	void DrawThrowPath();
	/*! Show the Throw Path before being actually thrown by the given power, originPosition and destination position */
	void SetThrowPathInfo(float force,CCPoint originPosition, CCPoint destPosition);

	void ResetThrowPathInfo(float dt);

	/*! temporary reset the ball to it's initial postion when it is out of view */
	//void TempReset();


	//functions for minspeed and max speed
	static float GetMaxSpeed();
	static float GetMinSpeed();



	/*! get the direction of ball */
	EThrowDirection GetBallDirection();

	CCPoint GetInitialTouchPosition();
	void SetInitialTouchPosition(CCPoint newTouchPos);

	/*! Calculate the minimum Speed , based on the distance of handes */
	static void CalculateSpeedBoundriesBaseOnLength(float deltaX)
	{
		//TODO: wtf +2 needed
		minSpeed = sqrt( GRAVITY * abs(deltaX))+ 2;
		//TODO: calculate maxSpeed
		maxSpeed = sqrt(GRAVITY *abs(deltaX)/sin(2 *MAX_THROW_RAD));
	}

	//taylor formula
	float TaylorFormulaCalculate(float angle);
};





