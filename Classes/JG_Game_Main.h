#ifndef __MainGame_SCENE_H__
#define __MainGame_SCENE_H__


#include "cocos2d.h"

#include "JG_Hand.h"
#include "JG_Ball.h"
#include "JG_Fruit.h"
#include "JG_Enemy_Base.h"
#include "JG_ScorePopup.h"
#include "JG_Path.h"
#include "JG_Factory_Base.h"
#include "JG_Factory_Enemy.h"

#include <vector>



#include  "JG_Game_HUD.h"
#include "JG_GUI_Bar.h"
using namespace cocos2d;


class JG_Game_HUD;
class JG_Ball;
class JG_Hand;
class JG_Fruit;
class JG_ScorePopup;
class JG_Path;
class JG_Enemy_Base;
class JG_Factory_Base;

#define GRAVITY CCDirector::sharedDirector()->getWinSize().height * 0.6

// define how many touches can be supported at the same time
#define TOUCH_COUNT 2

#define MAX_LIFE_COUNT 5

#define MAX_TOUCH_DURATOIN 0.50f
// throw force is relative to screen height
#define THROW_FORCE_BASE_ON_SREEN 0.7
//factor for minimum touch lenght tolerance
#define MIN_TOUCH_LENGTH_FACTOR 0.3

#define GAME_SCALE 0.6

#define DISCRETE_PARTS_COUNT 4


struct SEnemyTypes
{
	JG_Factory_Base * factory;
	int currentChance;
	int chanceIncreasePerRound;
};

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
	
	std::vector<SEnemyTypes> enemyTypes;
	
	JG_Hand* leftHand;
	JG_Hand* rightHand;
	CCArray* handsArray;
	CCArray* handsPowerBarArray;

	CCArray* ballsArray;

	CCArray* fruitsArray;

	CCArray* pathsArray;

	CCSprite* tempDestination;

	JG_Enemy_Base* tempEnemy;
	
	// store touch infos. 
	STouchInfo touchInfos[TOUCH_COUNT];

	//calculating Intial values of Throw Power
	float maxThrowPower;
	float maxTouchLenght;

	// trace texture for drawing throw paths
	CCTexture2D* tracePointTexture;
	CCTexture2D* traceLivePointTexture;



	float chosenPathPower;
	float powerRange;
	float discretedValue;
	float actualMinPower;
	/*! calculate and initials some variables for calculating throw power */
	void InitialThrowPowerVariables();
	/*! return proper discrete value */
	float DiscretedPowerValueGen(float input,JG_Ball* ball, bool bIsDemo = false);

	bool bIsGameInited; 



	int ballCounter;
	int prevballCounter;
	
	/* ! Manages Ball Score for combos */
	void ManageBallComboScore(JG_Ball* ball);
	void ManageFruitScore(JG_Fruit * fruit,JG_Ball * ball);

	template<class enemyClass>
	SEnemyTypes CreateEnemyType(int baseChance,int chaceIncrease);
	
	
public:
	
	JG_Game_Main(void);
	virtual ~JG_Game_Main(void);

	 // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

    /*! Initial the game state */
	void InitGame();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

	CCSize screenSize ;
	// refrenece to the Game HUD
	JG_Game_HUD * gameHUD;


	/*! finds best ball in balls that are touched in a hand */
	JG_Ball* FindBestBallMatching(JG_Hand*  hand);

	/**************** game rule members *************/
	int lifeCount;
	int score;
	/**************** /game rule members *************/

	

	/**************** game rule methods *************/

	/*! this event is called when a ball is lost ( for now when it is out of screen ) */
	//TODO:change the name to OnBallLost
	void OnBallLost(JG_Ball* lostBall);
	/*! this event is called when two balls are collided */
	void OnBallsCollide(JG_Ball* ballOne,JG_Ball* ballTwo);
	/*! this event is called when a ball is collided with a fruit*/
	void OnFruitHit(JG_Fruit* fruit, JG_Ball* ball);
	/*! this event is called when fruit is out of screen */
	void OnFruitLost(JG_Fruit* fruit);
	/*! this event is called when ball is successfully throwed*/
	void OnBallThrow(JG_Ball* ball);

	/*!this event called when a ball is collided with an enemy*/
	//void OnEnemyHit(JG_E


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

	
	/*! handles ball removing */
	void RemoveBallFromScreen(JG_Ball* ball);
	/*! Remove all balls from screen */
	void RemoveAllBallsFromScreen();
	/*! Adding new ball to screen */
	void AddBallToScreen();
	void TempAddBall(float time);


	void RemoveFruitFromScreen(JG_Fruit* fruit);
	void RemoveAllFruitsFromScreen();

	void AddFruitToScreen();
	void TempAddFruitToScreen(float time);

	void CheckBallCollisionWithHand();
	
	/**************** /game rule methods *************/



	/********************* Overwritten COCOS2D functions ********************/

    // implement the "static node()" method manually
    CREATE_FUNC(JG_Game_Main);
	    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

	void menuPauseCallBack(CCObject* pSender);

	/*! Handles the beginning of the touch */
	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* event);

	/*! Handles the movement of the touch */
	virtual void ccTouchesMoved(CCSet* pTouches, CCEvent* event);

	/*! Handles the end of the touch */
	virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* event);

	/*! update function */
	void update(float dt);
	void draw();

	/********************* /COCOS2D functions ********************/



	/*********************** Ball Touch handlers *******************/

	/*! defined to handle initiation of touch */
	void BallTouchHandler_Init(CCTouch* touch);
	/*! check direction of the ball */
	void BallTouchHandler_CheckDirection(unsigned int index);
	/*! handling the throwing of the ball */
	void BallTouchHandler_End(unsigned int index);
	/*! it handle the time player hold his touch on the screen */
	void BallTouchHandler_CheckTime(float dt);
	/*! to calculate the force of the touch */

	/*! Set Direction For Ball for the give index in touchInfos.
	 * @return : whether direction was valid or not 
	 */
	bool SetTouchDirectionForBall(int index);

	/*********************** /Ball Touch handlers *******************/


	/************************** Touch info methods ******************/
	/*! search through toucheInfos and return index for related ball */
	int GetTouchInfoIndexByBall(JG_Ball* ball);
	/*! Set an empty touchinfo with a new info*/
	void SetTouchInfo(CCTouch* touch, JG_Hand* hand,JG_Ball* ball);
	/*! Reset an touchinfo with the given index in touchInfos */
	void ResetTouchInfo( int index);
	/*! Reset an touchinfo with the given ball in touchInfos */
	void ResetTouchInfoByBall(JG_Ball* ball);

	/************************** /Touch info methods ******************/

	/*! draw the availble paths for balls throwing */
	//NOTE: shall it be here or there must be in another class ? 
	void DrawThrowPaths();
	/*! draw throw path based on given power */
	void DrawThrowPathByPower(float _power , bool mustHighlight);
	/*! updates hands' power bars */ 
	void UpdateHandPowerBar();
	/*! Update Ball Throw Trace for all touched balls */
	void UpdateBallThrowTrace();
	
	
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

	/* Calculate the throw power for a touched ball 
	 * @Param unsigned int index : the index of the ball 
	 * @Param bool bIsDemo : the flag the set whether must affect the ball or not (true: don't affect the ball)
	 */
	float CalculateThrowPower(unsigned int index, bool bIsDemo = false);
	
	/*! returns  maxThrow Power */
	float GetMaxThrowPower();
	/*! returns actual min power */
	float GetActualMinPower();
	/*! returns balls array */
	CCArray* GetBallArray();
	/*! returns maxThrowPower */ 



	






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

	/*! draw throw path based on given power */
	void DrawThrowPathByPower(float _power,int level);


	/*! check to see if a ball may be throw in this curve! */
	bool checkCurvesLife(float power);

	/*! checks throw path for each ball and activated them */
	void CheckBallsThrowPath();

	
	/*!checks whether distance of the two point are lesser than distance or not*/
	//TODO: find a better name
	bool ArePointsColliding (CCPoint point1,CCPoint point2,float distance);
	
	float absf(float );

	inline float getSign(float num)
	{
		return num/abs(num);
	}
	


	





};

#endif // __MainGame_SCENE_H__


