#ifndef __MainGame_SCENE_H__
#define __MainGame_SCENE_H__



#include "cocos2d.h"
#include "JG_Enums.h"
#include "Defines.h"

#include "JG_Menu_Main.h"

#include "JG_Hand.h"
#include "JG_Ball.h"

#include "JG_ScorePopup.h"
#include "JG_Path.h"

#include "JG_Enemy_Base.h"
#include "JG_Enemy_Crow.h"
#include "JG_Enemy_QuickAttakc.h"
#include "JG_Factory_Base.h"
//#include "JG_AttackWave_Base.h"

#include "JG_Factory_AttackWave.h"
#include "JG_AttackWave_AllLinesSequential.h"
#include "JG_AttackWave_AllLinesTogether.h"
#include "JG_Factory_Enemy.h"

#include <vector>
#include  "JG_Game_GUI.h"
#include "JG_GUI_Bar.h"
#include "JG_Score_Handler.h"
#include <vector>




//here is to test score handling
#include "JG_Score_Handler.h"
#include "JG_Test_ScoreHandler.h"



using namespace cocos2d;



class JG_Menu_Main;
class JG_Game_GUI;
class JG_Ball;
class JG_Hand;
class JG_Fruit;
class JG_ScorePopup;
class JG_Path;
class JG_Enemy_Base;
//class JG_Factory_Base;
////class JG_Factory_AttackWave;
class JG_AttackWave_Base;
//class JG_Enemy_Crow;
class JG_AttackWave_AllLinesSequential;




// define how many touches can be supported at the same time
#define TOUCH_COUNT 2

#define MAX_LIFE_COUNT 5

#define MAX_TOUCH_DURATOIN 0.50f
// throw force is relative to screen height
#define THROW_FORCE_BASE_ON_SREEN 0.7
//factor for minimum touch lenght tolerance
#define MIN_TOUCH_LENGTH_FACTOR 0.3

#define DISCRETE_PARTS_COUNT 4

#define INIT_BALL_COUNT 5

//enum EEnemyBonus;


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
	
	JG_Score_Handler* scoreFileHandler;
	vector<ScoreTableRecord>* scoreTable;

	bool bIsGameInited; 

	
	
	JG_Hand* leftHand;
	JG_Hand* rightHand;
	CCArray* handsArray;

	CCArray* ballsArray;

	CCArray* fruitsArray;

	
	//CCSprite* tempDestination;

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
	void InitialBallVariablesVariables();
	/*! return proper discrete value */
	float DiscretedPowerValueGen(float input,JG_Ball* ball, bool bIsDemo = false);

	void LoadContents();
	void LoadEnemysContents();



	int ballCounter;
	int prevballCounter;
	



	template<class enemyClass>
	SEnemyTypes CreateEnemyType(int baseChance,int chaceIncrease);
	

	
	float attackWaveCount;

	int ballsToRewardCounter;
	int totalBallsRewarded;
	float calculateLostHealth();
	float initialTotalHealth;
	int healthsToRewardCounter;
	int totalhealthsRewarded;
	JG_AttackWave_Base* currentAttackWave  ;

public:

	std::vector<SEnemyTypes> enemyTypes;
	

	std::vector<JG_Factory_Base*> attackWaveTypes;

	CCArray* enemyArray;
	CCArray* pathsArray;
	
	JG_Game_Main(void);
	virtual ~JG_Game_Main(void);

	 // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  



    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

	CCSize screenSize ;
	// refrenece to the Game GUI
	JG_Game_GUI * gameGUI;


	/*! finds best ball in balls that are touched in a hand */
	JG_Ball* FindBestBallMatching(JG_Hand*  hand);


	//TODO: move them to private
	/**************** game rule members *************/
	int lifeCount;
	int playerScore;
	int rank;
	int reservedBallCount;
	/**************** /game rule members *************/
	

	/**************** game rule methods *************/

	/*! this event is called when a ball is lost ( for now when it is out of screen ) */
	//TODO:change the name to OnBallLost
	void OnBallLost(JG_Ball* lostBall);
	//when path is destroyed
	void OnPathLost(JG_Path* path);

	void OnEnemyLost(JG_Enemy_Base* enemy);
	/*! this event is called when two balls are collided */
	void OnBallsCollide(JG_Ball* ballOne,JG_Ball* ballTwo);
	/*! this event is called when a ball is collided with a fruit*/
	//when a ball hit an enemy
	void OnEnemyHit(JG_Enemy_Base* enemy, JG_Ball* ball);

	void DamagePath(JG_Path* ,float damage);

	float GetBallRadius(JG_Ball* ball);
	
	
	/*! this event is called when ball is successfully throwed*/
	void OnBallThrow(JG_Ball* ball);

	/*!this event called when a ball is collided with an enemy*/
	//void OnEnemyHit(JG_E

	/* ! Manages Ball Score for combos */
	void ManagePathScore(JG_Path* path);
	void ManageEnemyBonus(EEnemyBonus bonus);

	bool IsThereAnyBallLeft();
	bool IsThereAnyPathLeft();

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

	void HealPath();
	JG_Path* FindADestroyedPath();
	JG_Path* FindMostDamagedPath();

	void IncrementReservedBallCount();
	void DecrementReservedBallCount();
	void SetReservedBallCount(int newCount);
	void ReleaseBall(CCObject* pSender);


	
	/*! handles ball removing */
	void RemoveBallFromScreen(JG_Ball* ball);
	//remove path from screen
	void RemovePathFromScreen(JG_Path* path);
	/*! Remove all balls from screen */
	void RemoveAllBallsFromScreen();
	/*! Adding new ball to screen */
	void AddBallToScreen();
	void TempAddBall(float time);


	void RemoveAllEnemiesFromScreen();

	void RemoveEnemyFromScreen(JG_Enemy_Base* enemy);

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

	void touchBeginHandler(CCTouch* touch);
	void touchEndHandler(CCTouch* touch);

	/*! update function */
	void update(float dt);
	void draw();

	/********************* /COCOS2D functions ********************/



	/*********************** Ball Touch handlers *******************/

	/*! defined to handle initiation of touch */
	void BallTouchHandler_Init(JG_Hand* hand, CCTouch* touch);
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
	//void DrawThrowPaths();
	/*! draw throw path based on given power */
	//void DrawThrowPathByPower(float _power , bool mustHighlight);
	/*! updates hands' power bars */ 
	//void UpdateHandPowerBar();
	/*! Update Ball Throw Trace for all touched balls */
	//void UpdateBallThrowTrace();
	
	int GetPathLevelByPower(float pathPower);
	


	/*! End of the game */
	void EndRound();
	/*! Pausing the game */
	void PauseGame();
	/*! Exit the game */
	void ExitGame();
	/*! Exit To Main Menu */
	void ExitToMainMenu();
	/*! Resuming the game */
	void ResumeGame();
	/*! Reseting the game */
	void ResetGame();
	//reset path

	void HandlePauseGame(CCObject* pSender);
	void HandleExitGame(CCObject* pSender);
	void HandleExitToMainMenu(CCObject* pSender);
	void HandleResumeGame(CCObject* pSender);
	void HandleResetGame(CCObject* pSender);

	void HandleEndRoundScreenResetGame(CCObject* pSender);
	void HandleEndRoundScreenExitToMainMenu(CCObject* pSender);

	

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

	

	inline float getSign(float num)
	{
		return num/JG_abs(num);
	}
	


	void CheckLoseCondition();

	/* game difficulty methods */

	/* initials parameters related to difficulty control */
	void InitGame_difficultyControl();
	/* returns attackwave type to be created */
	int getAttackWaveType();
	/* manage difficulty function */
	void ManageDifficulty();
	void initialNewAttackWave(float dt);
	/* when an attackwave (currentAttackWave) finishes its job this event occures */
	void onAttackWaveFinished();
	/* return number of currently available paths */
	int getAvailablePathCount();
	/* reset parameters related to difficulty control*/
	void resetDifficulty();
	/* manages rewards */
	void manageRewards(float dt);
	/* when a attack wave (currentAttackWave) rewards a ball to the user this event happens 
	* @this event dicreases number of balls calculated to be rewarded 
	*/
	void onBallRewarded();
	/* returns number of balls to be rewarded to the user */
	int getBallsToRewardCount();



	bool IsPlayerGetHighScore();
	int	DeterminePlayerRank();
	void InsertPlayerHighScore(CCString _playerName, int _rank, int _score);

	/* when a attack wave (currentAttackWave) rewards a health bonus to the user this event happens 
	* @this event dicreases number of healths calculated to be rewarded 
	*/
	void onHealthRewarded();
	/* returns number of balls to be rewarded to the user */
	int getHealthsToRewardCount();

	std::vector<SEnemyTypes> getEnemyTypes();
	CCArray* getpathsArray();
	void addEnemy(JG_Enemy_Base*);
	///////////////////////////////////////////////////////////////////





	//Sound Setting Function
	/*void playMusic(CCString backsound);
	void stopMusic();
	void resumeMusic();
	void pauseMusic();*/
	void playSoundEffect(CCString effectsound);

	void TestOutOfRangeRankForHighScore();

private:
	void InitialLoadingScreen();
	void InitialGameTimer(float dt);

	void InitialGame();

	void CreateHands();

	void CreateBackGround();

	void CreateGUI();

	void InitialCallBacks();

	void InitialPaths();

	void InitialEnemies();

	/*! Initial the round state */
	void InitRound();

	void CreateBalls();

};

#endif // __MainGame_SCENE_H__


