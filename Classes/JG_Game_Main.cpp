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
	gameHUD->setPosition(CCPointZero);
	this->addChild(gameHUD,100);
	//gameHUD->draw();

	/*********************** Background **************************/
	CCSprite * backGround = CCSprite::create("background2.png");

	backGround->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	this->addChild(backGround,-100);
	/************************* /Background *************************/



	/************************** Hands ***********************************/
	rightHand = JG_Hand::CreateHand(this,ccp(screenSize.width * 0.85 ,screenSize.height * 0.15),"RightHand.png");
	leftHand = JG_Hand::CreateHand(this,ccp(screenSize.width * 0.15,screenSize.height * 0.15),"LeftHand.png");

	


	handsArray= CCArray::create(rightHand,leftHand,NULL);
	handsArray->retain();
	//bar setting
	
	handsPowerBarArray=CCArray::create(JG_GUI_Bar::CreateBar(rightHand->getPosition()+ CCPointMake(0,0),100)
		,JG_GUI_Bar::CreateBar(leftHand->getPosition()+ CCPointMake(-10,10),100)
		,NULL);
	handsPowerBarArray->retain();
	
	for( int i = 0 ; i<handsArray->count();i++)
	{
		this->addChild((CCNode*)handsArray->objectAtIndex(i));
		this->addChild((CCNode*)handsPowerBarArray->objectAtIndex(i),2);
	}
	/*************************** /Hands *************************************/


	InitGame();
	this->setTouchEnabled(true);
    //test
	//TestMultiTouch();
	return true;

}

void JG_Game_Main::InitGame()
{
	

	/****************************** Balls ************************************/
	JG_Ball::CalculateSpeedBoundriesBaseOnLength(rightHand->getPositionX()-leftHand->getPositionX());
	JG_Ball::InitialBallLevelInformation();
	//it is import we call calculatethrowpower after jg_ball calculate
	CalculateInitialThrowPowers();
	// initing  one ball for test
	ballsArray=CCArray::create();
	ballsArray->retain();



	//TempAddBall(0);
	this->schedule(schedule_selector(JG_Game_Main::TempAddBall),1.75,2,1.5);
	

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
}

void JG_Game_Main::TempAddBall(float dt)
{
	AddBallToScreen();
		
}
void JG_Game_Main::update(float dt)
{
	BallTouchHandler_CheckTime(dt);
	//UpdateHandPower();
	UpdateBallThrowTrace();
	//TestSingleTouch();
	
}

/* This function first iterate through touches to find with wich hand they are colliding.
	Then for each hand that is touch, finds wich ball is colliding with it.
*/
void JG_Game_Main::BallTouchHandler_Init(CCTouch* touch)
{
	CCPoint tap = touch->getLocation();
	JG_Hand * currentHand;
	float criticalTime;


	for(int j = 0 ; j< handsArray->count();j++)
			{
				currentHand = (JG_Hand*)handsArray->objectAtIndex(j);
				//Checking if tap is colliding with any of hands
				if(ArePointsColliding(tap,currentHand->getPosition(),currentHand->GetRadius()))
				{
					criticalTime = 0.0;
					JG_Ball *tempBall;
					JG_Ball *criticalBall =0;
					//find the most critical ball (if exists) colliding with the ball
					//most critical ball is the ball witch will be lost befor other balls
					for (int k=0 ; k<ballsArray->count() ; k++)
					{
						tempBall=(JG_Ball *) ballsArray->objectAtIndex(k);
						if(ArePointsColliding(tempBall->getPosition(),currentHand->getPosition(),currentHand->GetRadius()))
						{
							if(tempBall->GetBallDirection() == EDir_LeftHandToUp || tempBall->GetBallDirection() == EDir_RightHandToUp)
							{
								if( (tempBall->getPositionY()/ tempBall->getCurrentSpeedY()) <criticalTime  )
								{
									criticalBall = tempBall;
									criticalTime = (tempBall->getPositionY()/ tempBall->getCurrentSpeedY()) ;
								}
							}
							
							else if(tempBall->GetBallDirection() == EDir_LeftHandToRight)
							{
								if( ( (screenSize.width - tempBall->getPositionX())/tempBall->getCurrentSpeedX()) <criticalTime)
								{
									criticalBall = tempBall;
									criticalTime = (screenSize.width - tempBall->getPositionX())/tempBall->getCurrentSpeedX();
								}
							}

							else if(tempBall->GetBallDirection() == EDir_RighHandtToLeft)
							{
								if(  (tempBall->getPositionX() / tempBall->getCurrentSpeedX()) < criticalTime)
								{
									criticalBall = tempBall;
									criticalTime = (tempBall->getPositionX() / tempBall->getCurrentSpeedX());

								}
							}
						
						}// end of ball collision cheking
					}// end of ball looping
				if(criticalBall != 0)	
				{
					SetTouchInfo(touch,currentHand,criticalBall);
				}

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
	touchInfos[index].ball->Throw(CalculateThrowPower(index),destHand->getPosition());

	//AddScore(touchInfos[index].ball->GetBallScore());
	ManageBallScore(touchInfos[index].ball);
	
	ResetTouchInfo(index);
}
float JG_Game_Main::CalculateThrowPower(unsigned int index, bool bIsDemo)
{
	//float holdingTime=MAX_TOUCH_DURATOIN- touchInfos[index].remainingTime;
	
	
	float touchLenght=abs(touchInfos[index].hand->getPositionY()-touchInfos[index].touch->getLocation().y);
	float currentPower=(touchLenght/maxTouchLenght)*maxThrowPower;
	
	return DiscretedPowerValueGen(currentPower,touchInfos[index].ball,bIsDemo);

}

float JG_Game_Main::DiscretedPowerValueGen(float input,JG_Ball* ball, bool bIsDemo)
{
	input = clampf(input,actualMinPower,maxThrowPower);

	
	input-=actualMinPower;
	//CCLOG("max value %f", GetMaxThrowPower());
	//CCLOG("min value %f", actualMinPower);
	//CCLOG("discrete value %f", (floor(input/range)*range)+actualMinPower);
	float powerLevel=floor(input/powerRange);

	disCretedValue=powerLevel*powerRange+actualMinPower;

	CCLOG("power level : %f",powerLevel);

	// set ball level only when it is thrown up
	if(!bIsDemo)
	{
		if(ball->GetBallDirection()== EDir_LeftHandToRight
			|| ball->GetBallDirection() ==EDir_RighHandtToLeft)
			ball->SetBallLevel(powerLevel);
	}
	return disCretedValue;
}

float JG_Game_Main::GetActualMinPower()
{
	return actualMinPower;
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

int JG_Game_Main::GetTouchInfoIndexByBall(JG_Ball* ball)
{
	for (int i=0;i<TOUCH_COUNT;i++)
	{
		if (touchInfos[i].ball==ball)
		{
			return i;
		}
	}
	return -1;

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
			touchInfos[i].initialTouchPosition=touch->getLocation();
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
void JG_Game_Main::ResetTouchInfoByBall(JG_Ball* ball)
{
	for (int i=0;i<TOUCH_COUNT;i++)
	{
		if (touchInfos[i].ball==ball)
		{
			ResetTouchInfo(i);
			return;
		}
	}
}



void JG_Game_Main::ManageBallScore(JG_Ball * ball)
{
	//NOTE: for better performance instead of dynamicaly searching for ball Levels we can store it someWhere
	JG_Ball * tempBall;
	int scoreMuliplier= 0;
	for( int i = 0 ; i< ballsArray->count(); i++)
	{
		if(((JG_Ball*)ballsArray->objectAtIndex(i))->GetBallLevel()== ball->GetBallLevel())
			scoreMuliplier++;
	}
	AddScore(ball->GetBallScore() *scoreMuliplier);
	
}

bool JG_Game_Main::ArePointsColliding(CCPoint point1,CCPoint point2,float radius)
{
	return point1.getDistance(point2)<radius;
}

void JG_Game_Main::BallLost(JG_Ball* lostBall)
{
	//CCLog("BallLOst",0);
	int touchInfoIndex = GetTouchInfoIndexByBall(lostBall);
	if(touchInfoIndex!= -1)
		BallTouchHandler_End(touchInfoIndex);
	else
	{
		RemoveBallFromScreen(lostBall);
		DecrementLifeCount();
		if(lifeCount>0)
		{
			AddBallToScreen();

		}
	}
	
	
}

int JG_Game_Main::GetScore()
{
	return score;
}
void JG_Game_Main::SetScore( int newScore)
{
	score = newScore;
	gameHUD->UpdateScore();

}

void JG_Game_Main::AddScore(int amount)
{

	score+= amount;
	gameHUD->UpdateScore();

}
void JG_Game_Main::ReduceScore(int amount)
{
	score-= amount;
	gameHUD->UpdateScore();
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
	if(lifeCount<=0)
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
	randomX=CCRANDOM_0_1()*screenSize.width;
	randomY=CCRANDOM_0_1()*screenSize.height;
	
	testPoint.setPoint(randomX,randomY);
	testPoint= CCDirector::sharedDirector()->convertToGL(testPoint);

	testTouch->setTouchInfo(1,testPoint.x,testPoint.y);
	
	ccTouchesMoved(testTouchSet,NULL);
	ccTouchesEnded(testTouchSet,NULL);
	
}

void JG_Game_Main::TestMultiTouch()
{
	//TODO: implement TestMultiTouch
	TestMultiTouchesSet=CCSet::create();
	TestMultiTouchesSet->retain();

	this->schedule(schedule_selector(JG_Game_Main::TestMultiTouch_InitiTouchGen),CCRANDOM_0_1()*0.01,1,0);
	this->schedule(schedule_selector(JG_Game_Main::TestMultiTouch_MovementTouchGen),CCRANDOM_0_1()*0.01,1,0);
	this->schedule(schedule_selector(JG_Game_Main::TestMultiTouch_EndGen),CCRANDOM_0_1()*0.5,1,0);

	
}

void JG_Game_Main::TestMultiTouch_InitiTouchGen(float dt)
{
	CCTouch* testTouch;
	testTouch = new CCTouch();
	float randomX;
	float randomY;
	CCPoint testPoint;
	
	this->unschedule(schedule_selector(JG_Game_Main::TestMultiTouch_InitiTouchGen));
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

	testTouch->setTouchInfo(int(CCRANDOM_0_1() * 5600),testPoint.x,testPoint.y);
	//CCPoint temp = testTouch->getLocation();
	
	CCSet* testTouchSet= CCSet::create();
	testTouchSet->addObject(testTouch);

	ccTouchesBegan(testTouchSet,NULL);
	
	TestMultiTouchesSet->addObject(testTouch);
	
	//scheduleOnce(schedule_selector(JG_Game_Main::TestMultiTouch_InitiTouchGen),CCRANDOM_0_1()*0.1);
	schedule(schedule_selector(JG_Game_Main::TestMultiTouch_InitiTouchGen),CCRANDOM_0_1()*0.01,1,0);

}

void JG_Game_Main::TestMultiTouch_MovementTouchGen(float dt)
{
	float randomX;
	float randomY;
	CCTouch* touch;
	CCPoint testPoint;

	this->unschedule(schedule_selector(JG_Game_Main::TestMultiTouch_MovementTouchGen));
	
	for(CCSetIterator i = TestMultiTouchesSet->begin(); i != TestMultiTouchesSet->end(); i++) 
	{
		touch = (CCTouch *) (*i);
		CCPoint temp=touch->getLocation();
		randomX=temp.x;
		randomY=temp.y;
		//generating random locaiton
		randomX+= CCRANDOM_0_1() *100;
		randomY+= CCRANDOM_0_1() *100;

		testPoint.setPoint(randomX,randomY);
		testPoint= CCDirector::sharedDirector()->convertToGL(testPoint);

		touch->setTouchInfo(touch->getID(),testPoint.x,testPoint.y);
		
	
	}
	ccTouchesMoved(TestMultiTouchesSet,NULL);

	//scheduleOnce(schedule_selector(JG_Game_Main::TestMultiTouch_MovementTouchGen),CCRANDOM_0_1()*0.1);

	schedule(schedule_selector(JG_Game_Main::TestMultiTouch_MovementTouchGen),CCRANDOM_0_1()*0.01,1,0);

}
void JG_Game_Main::TestMultiTouch_EndGen(float dt)
{

	int TempIndex=CCRANDOM_0_1()* TestMultiTouchesSet->count();
	CCSet* temp=CCSet::create();
	int random=CCRANDOM_0_1()*TestMultiTouchesSet->count();
	for(int i=0;i<random;i++)
		temp->addObject((CCTouch*)TestMultiTouchesSet->anyObject());

	this->unschedule(schedule_selector(JG_Game_Main::TestMultiTouch_EndGen));

	ccTouchesEnded(temp,NULL);

	//scheduleOnce(schedule_selector(JG_Game_Main::TestMultiTouch_EndGen),CCRANDOM_0_1()*0.1);

	schedule(schedule_selector(JG_Game_Main::TestMultiTouch_EndGen),CCRANDOM_0_1()*0.1,1,0);

}

void JG_Game_Main::PauseGame(CCObject* pSender)
{
	gameHUD->SetPauseScreen(true);
	CCDirector::sharedDirector()->pause();
}

void JG_Game_Main::ExitGame(CCObject* pSender)
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

void JG_Game_Main::ResumeGame(CCObject* pSender)
{
	CCDirector::sharedDirector()->resume();
	gameHUD->SetPauseScreen(false);
}

void JG_Game_Main::ResetGame(CCObject* pSender)
{
	RemoveAllBallsFromScreen();
	InitGame();
	ResumeGame(pSender);
}

void JG_Game_Main::EndGame()
{

//	CCTimer::timerWithTarget( this,SEL_CallFuncO(JG_Game_Main::RestartGame),2);
	//CCTimer::timerWithTarget(

	//ballsArray->autorelease();

}

void JG_Game_Main::RemoveAllBallsFromScreen()
{
	JG_Ball* tempBall;
	int temp = ballsArray->count();

	while(ballsArray->count()>0)
	{
		RemoveBallFromScreen((JG_Ball*)ballsArray->randomObject());
	}

}


void JG_Game_Main::RemoveBallFromScreen(JG_Ball* ball)
{
	
	ballsArray->removeObject(ball,false);
	removeChild(ball,true);
	CC_SAFE_RELEASE(ball);
}



void JG_Game_Main::AddBallToScreen()
{
	JG_Ball * newBall = JG_Ball::CreateBall(this
		,ccp(leftHand->getPositionX()
		,screenSize.height * 0.5 + CCRANDOM_0_1() *screenSize.height * 0.5)
		,EDir_RightHandToUp
		,DISCRETE_PARTS_COUNT);
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


//calculating hands power
void JG_Game_Main::UpdateHandPower()
{
	((JG_GUI_Bar*)handsPowerBarArray->objectAtIndex(0))->SetBarScale(0);
	((JG_GUI_Bar*)handsPowerBarArray->objectAtIndex(1))->SetBarScale(0);
	for (int i=0;i<TOUCH_COUNT;i++)
	{
		if(touchInfos[i].touch!=NULL)
		{
			if(handsArray->objectAtIndex(0)==touchInfos[i].hand)
				((JG_GUI_Bar*)handsPowerBarArray->objectAtIndex(0))->SetBarScale(CalculateThrowPower(i)*2);
			else
				((JG_GUI_Bar*)handsPowerBarArray->objectAtIndex(1))->SetBarScale(CalculateThrowPower(i)*2);
		}
		
	}
}

void JG_Game_Main::UpdateBallThrowTrace()
{
	for (int i=0;i<TOUCH_COUNT;i++)
	{
		if(touchInfos[i].touch!=NULL && touchInfos[i].bIsDirValid
			&& touchInfos[i].ball->moveMode==EMove_Straight)
		{
				//CCLog("WTTTTTTTTTTTTTTTF");
				if(touchInfos[i].hand== rightHand)
					touchInfos[i].ball->SetThrowPathInfo(CalculateThrowPower(i),rightHand->getPosition(),leftHand->getPosition());
				else
					touchInfos[i].ball->SetThrowPathInfo(CalculateThrowPower(i),leftHand->getPosition(),rightHand->getPosition());

		}
		
	}

}
void JG_Game_Main::CalculateInitialThrowPowers()
{
	
	maxThrowPower=(JG_Ball::GetMaxSpeed()/JG_Ball::GetMinSpeed()-1);
	maxThrowPower*=(1+MIN_TOUCH_LENGTH_FACTOR);
	maxTouchLenght=THROW_FORCE_BASE_ON_SREEN*screenSize.height-rightHand->getPositionY();
	
	//min power that we can have
	actualMinPower=maxThrowPower*MIN_TOUCH_LENGTH_FACTOR;
	//range of power between max and min
	powerRange=(maxThrowPower-actualMinPower)/DISCRETE_PARTS_COUNT;
	
	//CCLOG("Max maxThrowPower Length %f",maxThrowPower);
	
}