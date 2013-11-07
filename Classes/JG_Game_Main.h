#ifndef __MainGame_SCENE_H__
#define __MainGame_SCENE_H__

#include "cocos2d.h"

#include "JG_Hand.h"
#include "JG_Ball.h"

#include  "JG_Game_HUD.h"
using namespace cocos2d;

class JG_Game_HUD;
class JG_Ball;


// define how many touches can be supported at the same time
#define TOUCH_COUNT 2

#define MAX_LIFE_COUNT 5
#define MAX_TOUCH_DURATOIN 0.25f
#define THROW_FORCE_BASE 100


struct STouchInfo
{
	CCTouch * touch;
	JG_Hand * hand;
	JG_Ball * ball;
	bool bIsDirValid;
	float remainingTime;
	CCPoint initialTimePosition;

	
	
};

/*! The main class for controlling the game */
class JG_Game_Main : public cocos2d::CCLayer
{
	JG_Hand* leftHand;
	JG_Hand* rightHand;
	CCArray* handsArray;

	CCArray* ballsArray;

	// store touch infos. 
	STouchInfo touchInfos[TOUCH_COUNT];

	JG_Game_HUD * gameHUD;


public:

	/**************** game rule members *************/
	int lifeCount;
	int score;
	/**************** /game rule members *************/

	/**************** game rule methods *************/

	/*! this method is called when a ball is lost ( for now when it is out of screen ) */
	void BallLost(JG_Ball* lostBall);

	int GetScore();
	void SetScore( int newScore);
	void AddScore(int amount);
	void ReduceScore(int amount);

	int GetLifeCount();
	void SetLifeCount( int newLifeCount);
	void DecrementLifeCount();
	void IncrementLifeCount();
	
	/**************** /game rule methods *************/

	CCPoint screenSize ;

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
	float CalculateThrowForce(unsigned int index);

	/*! update function */
	void update(float dt);

	/*!checks whether distance of the two point are lesser than radius or not*/
	//TODO: find a better name
	bool ArePointsColliding (CCPoint point1,CCPoint point2,float radius);

	// Set an empy touchinfo with new infos
	void SetTouchInfo(CCTouch* touch, JG_Hand* hand,JG_Ball* ball);
	// Reset an touchinfo with the given index in touchInfos

	void ResetTouchInfo( int index);
	// Set Direction For Ball for the give index in touchInfos. return whether direction was valid or not 
	bool SetTouchDirectionForBall(int index);
	
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

	void menuPauseCallBack(CCObject* pSender);

	/* handles ball removing */
	void RemoveBallFromScreen(JG_Ball* ball);

	void RemoveAllBallsFromScreen();
	
	void AddBallToScreen();
	inline float getSign(float num)
	{
		return num/abs(num);
	}


    
    // implement the "static node()" method manually
    CREATE_FUNC(JG_Game_Main);
	/*! a function to test single touch */
	void TestSingleTouch();
	/*! a function to test single touch */
	void TestMultiTouch();

	void EndGame();

	void PauseGame(CCObject* pSender);
	void ExitGame(CCObject* pSender);
	void ResumeGame(CCObject* pSender);
	void ResetGame(CCObject* pSender);


};

#endif // __MainGame_SCENE_H__


