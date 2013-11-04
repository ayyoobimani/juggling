#include "JG_Game_Main.h"


USING_NS_CC;

CCScene* JG_Game_Main::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    JG_Game_Main *layer = JG_Game_Main::create();

    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool JG_Game_Main::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	screenSize = CCDirector::sharedDirector()->getWinSize();

	//call update for every frame
	this->schedule(schedule_selector(JG_Game_Main::update));


	gameHUD = JG_Game_HUD::create(this);
	gameHUD->retain();
	this->addChild(gameHUD,100);
	//gameHUD->draw();

	/*********************** Background **************************/
	CCSprite * backGround = CCSprite::create("background.png");

	backGround->setPosition(ccp(screenSize.x/2,screenSize.y/2));
	this->addChild(backGround,-100);
	/************************* /Background *************************/



	/************************** Hands ***********************************/
	rightHand = JG_Hand::createWithFileName("RightHand.png",ccp(screenSize.x-50,40));
	leftHand = JG_Hand::createWithFileName("LeftHand.png",ccp(50,40));

	handsArray= CCArray::create(rightHand,leftHand,NULL);
	handsArray->retain();
	for( int i = 0 ; i<handsArray->count();i++)
	{
		this->addChild((CCNode*)handsArray->objectAtIndex(i),2);
	}
	/*************************** /Hands *************************************/


	/****************************** Balls ************************************/
	JG_Ball::CalculateSpeedBoundriesBaseOnLength(rightHand->getPositionX()-leftHand->getPositionX());

	// initing  one ball for test
	ballsArray=CCArray::create(JG_Ball::CreateBall(this,ccp(leftHand->getPositionX(),200),EDir_RightHandToUp)
		/*,JG_Ball::CreateBall(this,ccp(leftHand->getPositionX(),300),EDir_RightHandToUp)*/
		,NULL);
	ballsArray->retain();

	for( int i = 0 ; i<ballsArray->count();i++)
	{
		this->addChild((CCNode*)ballsArray->objectAtIndex(i),6);

	}

	/******************************** /Balls ************************************/


	for( int i = 0 ; i< TOUCH_COUNT ; i++)
	{
		touchInfos[i].ball = NULL;
		touchInfos[i].hand = NULL;
		touchInfos[i].touch = NULL;
		touchInfos[i].bIsDirValid = false;
	}


	SetLifeCount(MAX_LIFE_COUNT);
	SetScore(0);

	this->setTouchEnabled(true);
    return true;
}

void JG_Game_Main::update(float dt)
{
	BallTouchHandler_CheckTime(dt);
	//TestSingleTouch();
}

/* This function first iterate through touches to find with wich hand they are colliding.
	Then for each hand that is touch, finds wich ball is colliding with it.
*/
void JG_Game_Main::BallTouchHandler_Init(CCTouch* touch)
{
	CCPoint tap = touch->getLocation();
	JG_Hand * currentHand;
	for(int j = 0 ; j< handsArray->count();j++)
			{
				currentHand = (JG_Hand*)handsArray->objectAtIndex(j);
				//Checking if tap is colliding with any of hands
				if(ArePointsColliding(tap,currentHand->getPosition(),currentHand->GetRadius()))
				{
					JG_Ball *tempBall;
					//cheking if hand is colliding with a ball
					for (int k=0 ; k<ballsArray->count() ; k++)
					{
						tempBall=(JG_Ball *) ballsArray->objectAtIndex(k);
						if(ArePointsColliding(tempBall->getPosition(),currentHand->getPosition(),currentHand->GetRadius()))
						{
							SetTouchInfo(touch,currentHand,tempBall);
							
						
						}// end of ball collision cheking
					}// end of ball looping
				}// end of hand collision checking
			}// end of hand looping	
}

void JG_Game_Main::ccTouchesBegan(CCSet* pTouches, CCEvent* event)
{
	CCSetIterator i;
	CCTouch* touch;
	CCPoint tap;

	for( i = pTouches->begin(); i != pTouches->end(); i++) 
	{
		touch = (CCTouch*) (*i);
		if(touch) 
		{
			BallTouchHandler_Init(touch);
		}
	}
}

void JG_Game_Main::BallTouchHandler_CheckDirection(unsigned int index)
{
	if(!touchInfos[index].bIsDirValid)
	{
		touchInfos[index].bIsDirValid = SetTouchDirectionForBall(index);
	}
	else
	{
		touchInfos[index].bIsDirValid = SetTouchDirectionForBall(index);
		if(!touchInfos[index].bIsDirValid)
		{
			BallTouchHandler_End(index);
		}
	}
}

void JG_Game_Main::ccTouchesMoved(CCSet* pTouches, CCEvent* event)
{
	CCTouch* touch;
	for( CCSetIterator i = pTouches->begin(); i != pTouches->end(); i++)
	{
		touch = (CCTouch *) (*i);
		if(touch!=NULL)
		{
			for(int j = 0 ; j < TOUCH_COUNT; j++)
			{
				if(touchInfos[j].touch== touch)
				{
					BallTouchHandler_CheckDirection(j);
				}
			}
		}
	}
}

//TODO: clean up and make it more modular
bool JG_Game_Main::SetTouchDirectionForBall(int index)
{
	JG_Ball* currentBall = touchInfos[index].ball;
	JG_Hand* currentHand = touchInfos[index].hand;
	// find direction based on ball's initialTouchPosition and current touch position
	CCPoint direction=(touchInfos[index].touch->getLocation()- currentBall->GetInitialTouchPosition());

	// calculate the direction in Radian
	//********************* TODO: find a better soloution for this ****************/
	float directionRad = atan(direction.y/direction.x);
	if(getSign(direction.x)<0 )
		directionRad+= CC_DEGREES_TO_RADIANS(180);

	if(directionRad<0)
		directionRad+= CC_DEGREES_TO_RADIANS(360);
	/*****************************************************************************/


	float directionDeg = CC_RADIANS_TO_DEGREES(directionRad);

	//NOTE: anything that is not mentioned if "if"s is not valid
	/************************* Checking Direction Validation **************************/
	//direction up
	if(directionDeg>45&& directionDeg<135)
	{
		if (currentBall->GetBallDirection() != EDir_LeftHandToUp && currentBall->GetBallDirection()!=EDir_RightHandToUp)
		{
			if(currentHand==leftHand && currentBall->GetBallDirection()==EDir_RighHandtToLeft)
			{
				return true;
			}
			else if (currentHand==rightHand && currentBall->GetBallDirection()==EDir_LeftHandToRight)
			{
				return true;
			}
		}
	}
	//direction left
	else if(directionDeg>135&&directionDeg<225)
	{	
		if (currentBall->GetBallDirection() == EDir_LeftHandToUp)
		{
			if (currentHand==rightHand)
			{
				return true;
			}
		}
	}
	//direction right
	else if(directionDeg>315||directionDeg<45)
	{	
		if (currentBall->GetBallDirection() == EDir_RightHandToUp)
		{
			if(currentHand==leftHand)
			{
				return true;
			}
		}
	}
	/************************* /Checking Direction Validation **************************/
	return false;
}

void JG_Game_Main::BallTouchHandler_CheckTime(float dt)
{
	for (int i=0; i <TOUCH_COUNT;i++)
	{
		if(touchInfos[i].touch!=NULL)
		{
			touchInfos[i].remainingTime-=dt;
			if (touchInfos[i].remainingTime<0)
			{
				BallTouchHandler_End(i);
			}
		}
	}
}
void JG_Game_Main::BallTouchHandler_End(unsigned int index)
{
	if(!touchInfos[index].bIsDirValid)
	{
		ResetTouchInfo(index);
		return;
	}
	JG_Hand * destHand;

	touchInfos[index].ball->setPosition(touchInfos[index].hand->getPosition());
	if(touchInfos[index].hand==leftHand)
		destHand=rightHand;	
	else
		destHand=leftHand;
	touchInfos[index].ball->Throw(CalculateThrowForce(index),destHand->getPosition());
	AddScore(touchInfos[index].ball->GetBallScore());
	
	ResetTouchInfo(index);
}
float JG_Game_Main::CalculateThrowForce(unsigned int index)
{
	return touchInfos[index].initialTimePosition.getDistance(touchInfos[index].touch->getLocation())/THROW_FORCE_BASE;
}

// for now just reset everything
void JG_Game_Main::ccTouchesEnded(CCSet* pTouches, CCEvent* event)
{
	CCTouch * touch;
	for( CCSetIterator i = pTouches->begin(); i != pTouches->end(); i++) 
	{
		touch =  (CCTouch*) (*i);
		if(touch!=NULL)
		{
			for( int j = 0 ; j<TOUCH_COUNT ; j++)
			{
				if(touchInfos[j].touch == touch)
				{
					BallTouchHandler_End(j);
					break;
				}
			}
		}
	}
}

void JG_Game_Main::SetTouchInfo(CCTouch* touch, JG_Hand* hand,JG_Ball* ball)
{
	for(int i = 0 ; i<TOUCH_COUNT ; i++)
	{
		if(touchInfos[i].touch==NULL)
		{
			ball->SetInitialTouchPosition(ball->getPosition());

			touchInfos[i].touch = touch;
			touchInfos[i].hand = hand;
			touchInfos[i].ball = ball;
			touchInfos[i].bIsDirValid = false;
			touchInfos[i].remainingTime=MAX_TOUCH_DURATOIN;
			touchInfos[i].initialTimePosition=touch->getLocation();
			return;
		}
	}
}

void JG_Game_Main::ResetTouchInfo(int index)
{
	touchInfos[index].touch = NULL;
	touchInfos[index].hand = NULL;
	touchInfos[index].ball = NULL;
	touchInfos[index].bIsDirValid = false;
}

bool JG_Game_Main::ArePointsColliding(CCPoint point1,CCPoint point2,float radius)
{
	return point1.getDistance(point2)<radius;
}

void JG_Game_Main::BallLost(JG_Ball* lostBall)
{
	CCLog("BallLOst",0);
	RemoveBallFromScreen(lostBall);
	DecrementLifeCount();
	if(lifeCount>=0)
		AddBallToScreen();
	
	
}

int JG_Game_Main::GetScore()
{
	return score;
}
void JG_Game_Main::SetScore( int newScore)
{
	score = newScore;
}

void JG_Game_Main::AddScore(int amount)
{

	score+= amount;
	gameHUD->UpdateScore();

}
void JG_Game_Main::ReduceScore(int amount)
{
	score-= amount;
}

int JG_Game_Main::GetLifeCount()
{
	return lifeCount;
}

void JG_Game_Main::SetLifeCount( int newLifeCount)
{
	lifeCount = newLifeCount;
}

void JG_Game_Main::DecrementLifeCount()
{
	--lifeCount;
	if(lifeCount<0)
		EndGame();
}

void JG_Game_Main::IncrementLifeCount()
{
	++lifeCount;
}

void JG_Game_Main::TestSingleTouch()
{
	CCTouch* testTouch;
	testTouch = new CCTouch();
	float randomX;
	float randomY;
	CCPoint testPoint;
	
	if (CCRANDOM_0_1()>0.5)
	{
		randomX=leftHand->getPositionX();
		randomY=leftHand->getPositionY();
	}
	else
	{
		randomX=rightHand->getPositionX();
		randomY=rightHand->getPositionY();
	}
	
	
	testPoint.setPoint(randomX,randomY);
	testPoint= CCDirector::sharedDirector()->convertToGL(testPoint);

	testTouch->setTouchInfo(1,testPoint.x,testPoint.y);
	CCPoint temp = testTouch->getLocation();
	
	CCSet* testTouchSet= CCSet::create();
	testTouchSet->addObject(testTouch);

	ccTouchesBegan(testTouchSet,NULL);

	//test_touch_move
	randomX=CCRANDOM_0_1()*screenSize.x;
	randomY=CCRANDOM_0_1()*screenSize.y;
	
	testPoint.setPoint(randomX,randomY);
	testPoint= CCDirector::sharedDirector()->convertToGL(testPoint);

	testTouch->setTouchInfo(1,testPoint.x,testPoint.y);
	
	ccTouchesMoved(testTouchSet,NULL);
	ccTouchesEnded(testTouchSet,NULL);
	
}

void JG_Game_Main::TestMultiTouch()
{
	//TODO: implement TestMultiTouch
	
	
	

}

void JG_Game_Main::EndGame()
{
	JG_Ball* tempBall;
	int temp = ballsArray->count();

	while(ballsArray->count()>0)
	{
		RemoveBallFromScreen((JG_Ball*)ballsArray->randomObject());
	}

	//ballsArray->autorelease();

}

void JG_Game_Main::RemoveBallFromScreen(JG_Ball* ball)
{
	
	ballsArray->removeObject(ball,false);
	removeChild(ball,true);
	CC_SAFE_RELEASE(ball);
}

void JG_Game_Main::AddBallToScreen()
{
	JG_Ball * newBall = JG_Ball::CreateBall(this,ccp(leftHand->getPositionX(),100+ CCRANDOM_0_1() *200),EDir_RightHandToUp);
	this->addChild(newBall,6);
	ballsArray->addObject(newBall);

}


void JG_Game_Main::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void JG_Game_Main::menuPauseCallBack(CCObject* pSender)
{
	if(!CCDirector::sharedDirector()->isPaused())
		CCDirector::sharedDirector()->pause();
	else
		CCDirector::sharedDirector()->resume();
}