#pragma once
#include "sprite_nodes\CCSprite.h"
#include "JG_Game_Main.h"
#include "actions\CCActionInterval.h"
#include "cocos2d.h"


using namespace cocos2d;

class JG_Game_Main;

#define GRAVITY CCDirector::sharedDirector()->getWinSize().height * 0.6

#define MAX_THROW_RAD (CC_DEGREES_TO_RADIANS(80))
#define BALL_SCALE 1.0

// Step counts for tracking Path of the Ball
#define BALL_PATH_TRACE_STEPS 30
// Interval time for tracking Path of the Ball
#define BALL_PATH_TRACE_INTERVALS 0.07 

#define BALL_PATH_TRACE_FADE_DELAY 1

#define MAX_BALL_LEVELS 6

enum EThrowDirection
{
	Dir_LeftHandToRight, Dir_RighHandtToLeft, Dir_LeftHandToUp, Dir_RightHandToUp
};

enum EMoveMode
{
	Move_Curve, Move_Straight
};

/*! The main class for handling ball movement */
class JG_Ball :
	public CCSprite
{
	/*!  ball Scores for each level */
	static int ballScoreByLevel[MAX_BALL_LEVELS];
	/*! ball Texture Names for each level */
	static CCString ballTextureNamesByLevel[MAX_BALL_LEVELS];
	/*! ball Textures for each level */
	static CCTexture2D* ballTexturesByLevel[MAX_BALL_LEVELS];

	int ballLevel;

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
	CCPoint throwPath_OriginPosition, throwPath_DestPosition;



	bool bMustShine;

	
	


public:
	JG_Ball(void);
	virtual ~JG_Ball(void);

	/*! Creating ball in a specific position and specific direction */
	static JG_Ball* CreateBall(JG_Game_Main* game,CCPoint initialPos,EThrowDirection initialDirection,int initialBallLevel);

	
	static float minSpeed;
	static float maxSpeed;

	//radius
	float radius;

	float currentSpeed;
	
	EThrowDirection ballThrowDirection;

	/*! radians of falling in EMove_Curve */
	float curveRadian;
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

	
	/*! Initial Throw movement variables */
	void Throw(float force,CCPoint destination);

	/*! process the movement of ball */
	void ProcessMove(float dt);

	/*! Handles the movement based on the current mode */
	void update(float dt);

	float GetNewSpeedByForce(float force);
	/*! Returns the new ThrowDirection based on the given previous direction */
	EThrowDirection GetNewThrowDirection(EThrowDirection preDir);
	/*! Returns the new MoveMove based on the given previous ModeMode */
	EMoveMode GetNewMoveMode(EMoveMode preMoveMode);

	/*! Calculate Curve Radian By the given Speed, Origin Position and destination position */
	static float CalculateCurveRad(float speed,CCPoint originPosition,CCPoint destPosition);

	void draw();
	/*! draws ball texture based on current ball level */
	void DrawBallTexture();
	void DrawThrowPath();
	/*! Show the Throw Path before being actually thrown by the given power, originPosition and destination position */
	void SetThrowPathInfo(float force,CCPoint originPosition, CCPoint destPosition);

	void ResetThrowPathInfo(float dt);

	/*! temporary reset the ball to it's initial postion when it is out of view */
	void TempReset(); 


	//functions for minspeed and max speed
	static float GetMaxSpeed();
	static float GetMinSpeed();

	void SetBallLevel(int newLevel);
	int GetBallLevel(){return ballLevel;}

	int GetBallScore();

	void OutOfScreen();

	//collision of balls checking
	void CheckCollisionWithBall();

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

	static void InitialBallLevelInformation()
	{
		ballScoreByLevel[MAX_BALL_LEVELS-1] = 50;
		ballScoreByLevel[MAX_BALL_LEVELS-2] = 100;
		ballScoreByLevel[MAX_BALL_LEVELS-3] = 200;
		ballScoreByLevel[MAX_BALL_LEVELS-4] = 400;
		ballScoreByLevel[MAX_BALL_LEVELS-5] = 800;
		ballScoreByLevel[MAX_BALL_LEVELS-6] = 1600;

		ballTextureNamesByLevel[MAX_BALL_LEVELS-1] = "Gem1.png";
		ballTextureNamesByLevel[MAX_BALL_LEVELS-2] = "Gem3.png";
		ballTextureNamesByLevel[MAX_BALL_LEVELS-3] = "Gem4.png";
		ballTextureNamesByLevel[MAX_BALL_LEVELS-4] = "Gem5.png";
		ballTextureNamesByLevel[MAX_BALL_LEVELS-5] = "Gem6.png";
		ballTextureNamesByLevel[MAX_BALL_LEVELS-6] = "Gem7.png";

		for( int i = 0 ; i<MAX_BALL_LEVELS; i++)
		{
			ballTexturesByLevel[i] = CCTextureCache::sharedTextureCache()->addImage(ballTextureNamesByLevel[i].getCString());
		}

	}

	//taylor formula
	//float TaylorFormulaCalculate(float angle);

	float GetCurrentSpeedX();
	float GetCurrentSpeedY();

	void SetShineVisibility(bool _newShine);

};





