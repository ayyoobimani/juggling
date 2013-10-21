//Program by : Sina Ghazizadeh
#ifndef __MainGame_SCENE_H__
#define __MainGame_SCENE_H__

#include "cocos2d.h"

#include "JG_Hand.h"
#include "JG_Ball.h"
using namespace cocos2d;


/*! The main class for controlling the game */
class JG_Main_Game : public cocos2d::CCLayer
{
	JG_Hand* leftHand;
	JG_Hand* rightHand;
	CCArray* handsArray;
	JG_Hand* currentHand;

	CCArray* ballsArray;
	
	JG_Ball* currentBall;

	CCPoint screenSize ;

	bool bDirIsSet;

public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    /*! Initial the game state */
	virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

	/*! Handles the beginning of the touch */
	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* event);
	/*! Handles the movement of the touch */
	virtual void ccTouchesMoved(CCSet* pTouches, CCEvent* event);
	/*! Handles the end of the touch */
	virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* event);
	/*! update function */
	void update(float dt);

	/*!checks whether distance of the two point are lesser than radius or not*/
	//TODO: find a better name
	bool ArePointsColliding (CCPoint point1,CCPoint point2,float radius);

	
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

	void menuPauseCallBack(CCObject* pSender);

	inline float getSign(float num)
	{
		return num/abs(num);
	}


    
    // implement the "static node()" method manually
    CREATE_FUNC(JG_Main_Game);
	/*! a function to test touch */
	void TestTouch();

};

#endif // __MainGame_SCENE_H__


