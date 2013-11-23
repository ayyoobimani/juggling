#ifndef __MainGame_SCENE_H__
#define __MainGame_SCENE_H__

#include "cocos2d.h"

#include "JG_Hand.h"
#include "JG_Ball.h"

#include  "JG_Game_HUD.h"
#include "JG_GUI_Bar.h"
using namespace cocos2d;

class JG_Game_HUD;
class JG_Ball;
class JG_Hand;


// define how many touches can be supported at the same time
#define TOUCH_COUNT 2

#define MAX_LIFE_COUNT 5
#define MAX_TOUCH_DURATOIN 0.50f
// throw force is relative to screen height
#define THROW_FORCE_BASE_ON_SREEN 0.7
//factor for minimum touch lenght tolerance
#define MIN_TOUCH_LENGTH_FACTOR 0.3

#define GAME_SCALE 0.7

#define DISCRETE_PARTS_COUNT 4.0

struct STouchInfo
{
	CCTouch * touch;
	JG_Hand * hand;
	JG_Ball * ball;
	bool bIsDirValid;
	float remainingTime;
	CCPoint initialTouchPosition;



	
	
};

/*! The main class for controlling the game */
class JG_Game_Main : public cocos2d::CCLayer
{
	JG_Hand* leftHand;
	JG_Hand* rightHand;
	CCArray* handsArray;
	CCArray* handsPowerBarArray;

	CCArray* ballsArray;
	
	// store touch infos. 
	STouchInfo touchInfos[TOUCH_COUNT];

	//calculating Intial values of Throw Power
	float maxThrowPower;
	float maxTouchLenght;

	

	/* ! Manages Ball Score for combos */
	void ManageBallScore(JG_Ball* ball);

	
	float powerRange;
	float disCretedValue;
	float actualMinPower;
	
	void CalculateInitialThrowPowers();
	/*! return proper discrete value */
	float DiscretedPowerValueGen(float input,JG_Ball* ball);
	
	

public:
	
	JG_Game_HUD * gameHUD;

	/**************** game rule members *************/
	int lifeCount;
	int score;
	/**************** /game rule members *************/

	//actual min power
	float GetActualMinPower();

	/**************** game rule methods *************/

	/*! this method is called when a ball is lost ( for now when it is out of screen ) */
	void BallLost(JG_Ball* lostBall);
/*! Return player Score */
	int GetScore();
	/*! Set player Score to a new value */
	void SetScore( int newScore);
	/*! Add a value to player score */
	void AddScore(int amount);
	/*! Reduce a value from player Score */
	void ReduceScore(int amount);
/*! Return The life count of player */
	int GetLifeCount();
	/*! Set the life count of player*/
	void SetLifeCount( int newLifeCount);
	/*! Decrement the life count of player */
	void DecrementLifeCount();
	/*! Increment the life count of player */
	void IncrementLifeCount();
	
	/**************** /game rule methods *************/

	CCSize screenSize ;

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  



    /*! Initial the game state */
	void InitGame();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

	/*! Handles the beginning of the touch */
	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* event);

	/*! Handles the movement of the touch */
	virtual void ccTouchesMoved(CCSet* pTouches, CCEvent* event);

	/*! Handles the end of the touch */
	virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* event);


	/*! defined to handle initiation of touch */
	void BallTouchHandler_Init(CCTouch* touch);
	/*! check direction of the ball */
	void BallTouchHandler_CheckDirection(unsigned int index);
	/*! handling the throwing of the ball */
	void BallTouchHandler_End(unsigned int index);
	/*! it handle the time player hold his touch on the screen */
	void BallTouchHandler_CheckTime(float dt);
	/*! to calculate the force of the touch */
	float CalculateThrowPower(unsigned int index);
	

	/*! update function */
	void update(float dt);

	/*!checks whether distance of the two point are lesser than radius or not*/
	//TODO: find a better name
	bool ArePointsColliding (CCPoint point1,CCPoint point2,float radius);

	/*! search through toucheInfos and return index for related ball */
	int GetTouchInfoIndexByBall(JG_Ball* ball);
	
	/*! Set an empty touchinfo with a new info*/
	void SetTouchInfo(CCTouch* touch, JG_Hand* hand,JG_Ball* ball);
	/*! Reset an touchinfo with the given index in touchInfos */
	void ResetTouchInfo( int index);
	/*! Reset an touchinfo with the given ball in touchInfos */
	void ResetTouchInfoByBall(JG_Ball* ball);
	/*! Set Direction For Ball for the give index in touchInfos. return whether direction was valid or not */
	bool SetTouchDirectionForBall(int index);
	
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

	void menuPauseCallBack(CCObject* pSender);

	

	/*! handles ball removing */
	void RemoveBallFromScreen(JG_Ball* ball);
	/*! Remove all balls from screen */
	void RemoveAllBallsFromScreen();
	/*! Adding new ball to screen */
	void AddBallToScreen();
	
	inline float getSign(float num)
	{
		return num/abs(num);
	}


	/*! returns maxThrowPower */ 
	float GetMaxThrowPower()
	{
		return maxThrowPower;
	}

    
    // implement the "static node()" method manually
    CREATE_FUNC(JG_Game_Main);
	/*! a function to test single touch */
	void TestSingleTouch();
	/*! a function to test single touch */
	CCSet* TestMultiTouchesSet;
	/*! Testing MultiTouch Handling */
	void TestMultiTouch();
	/*! Initial phase of multi touch testing */
	void TestMultiTouch_InitiTouchGen(float dt=0);
	/*! Movement phase of multi touch testing */
	void TestMultiTouch_MovementTouchGen(float dt=0);
	/*! End phase of multi touch testing */
	void TestMultiTouch_EndGen(float dt=0);
	/*! End of the game */
	void EndGame();
	/*! Pausing the game */
	void PauseGame(CCObject* pSender);
	/*! Exit the game */
	void ExitGame(CCObject* pSender);
	/*! Resuming the game */
	void ResumeGame(CCObject* pSender);
	/*! Reseting the game */
	void ResetGame(CCObject* pSender);
	
	void TempAddBall(float time);




	//power calculating 
	void UpdateHandPower();

	/*! Update Ball Throw Trace */
	void UpdateBallThrowTrace();

	


};

#endif // __MainGame_SCENE_H__


