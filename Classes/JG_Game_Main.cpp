#include "JG_Game_Main.h"


USING_NS_CC;

JG_Game_Main::JG_Game_Main()
{
	bIsGameInited = false;
}

JG_Game_Main::~JG_Game_Main()
{

}

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
	prevballCounter =0 ;

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
		this->addChild((CCNode*)handsArray->objectAtIndex(i),5);
		this->addChild((CCNode*)handsPowerBarArray->objectAtIndex(i),2);
	}
	/*************************** /Hands *************************************/

	JG_Ball::CalculateSpeedBoundriesBaseOnLength(rightHand->getPositionX()-leftHand->getPositionX());
	JG_Ball::InitialBallLevelInformation();
	//it is import we call calculatethrowpower after jg_ball calculate
	InitialThrowPowerVariables();

	/*************************** Paths ****************************/

	pathsArray = CCArray::create();
	pathsArray->retain();
	for( int i= 0 ; i< DISCRETE_PARTS_COUNT ; i++)
	{
		pathsArray->addObject(JG_Path::CreatePath(this
			,i * powerRange
			, leftHand->getPosition()
			, rightHand->getPosition()));
		this->addChild((CCNode*)pathsArray->objectAtIndex(i),2);
	}

	/*************************** /Paths ****************************/

	/************************* Enemy Factories ********************/

	enemyTypes.push_back(CreateEnemyType<JG_Enemy_Base>(6,2));

	/************************* /Enemy Factories ********************/


	
	tempDestination = new CCSprite();
	tempDestination->initWithFile("cross.png");
	tempDestination->retain();
	this->addChild((CCNode*) tempDestination);

	tempEnemy = (JG_Enemy_Base*) enemyTypes[0].factory->Create();
	tempEnemy->InitialEnemy(this,ccp(50,50));

	this->addChild((CCNode*) tempEnemy);

	InitGame_AttackWaves();


	InitGame();
	this->setTouchEnabled(true);
	//test
	//TestMultiTouch();
	return true;

	
}

void JG_Game_Main::InitGame_AttackWaves()
{
	attackWaveTypes.push_back(new JG_Factory_AttackWave<JG_AttackWave_AllLinesSequential>);
}

void JG_Game_Main::InitGame()
{


	tracePointTexture = CCTextureCache::sharedTextureCache()->addImage("deadStar.png");
	traceLivePointTexture = CCTextureCache::sharedTextureCache()->addImage("liveStar.png");

	/****************************** Balls ************************************/

	// initing  one ball for test
	ballsArray=CCArray::create();
	ballsArray->retain();

	fruitsArray= CCArray::create();
	fruitsArray->retain();


	//TempAddBall(0);
	//this->schedule(schedule_selector(JG_Game_Main::TempAddBall),1.75,2,1.5);
	//fruit
	//this->schedule(schedule_selector(JG_Game_Main::TempAddFruitToScreen),CCRANDOM_0_1());


	/******************************** /Balls ************************************/

	SetReservedBallCount(5);

	for( int i = 0 ; i< TOUCH_COUNT ; i++)
	{
		touchInfos[i].ball = NULL;
		touchInfos[i].hand = NULL;
		touchInfos[i].touch = NULL;
		touchInfos[i].bIsDirValid = false;
	}


	SetLifeCount(MAX_LIFE_COUNT);
	SetScore(0);
	bIsGameInited = true;
}



void JG_Game_Main::update(float dt)
{
	BallTouchHandler_CheckTime(dt);
	//UpdateHandPowerBar();
	//UpdateBallThrowTrace(); 

	//TestSingleTouch();
	CheckBallCollisionWithHand();
	CheckBallsThrowPath();

}

void JG_Game_Main::CheckBallsThrowPath()
{

	for( int i = 0; i < pathsArray->count() ; i++)
	{
		((JG_Path * ) pathsArray->objectAtIndex(i))->SetHighlight(false);
	}

	for( int i = 0 ; i< TOUCH_COUNT; i++)
	{
		if( touchInfos[i].ball!=NULL)
		{
			if(touchInfos[i].ball->GetBallDirection() == Dir_RighHandtToLeft
				|| touchInfos[i].ball->GetBallDirection() == Dir_LeftHandToRight)
			{
				int ballPath = CalculateThrowPower(i,true)/ powerRange;
				//CCLOG("power level : %f",ballPath);
				((JG_Path * ) pathsArray->objectAtIndex(ballPath))->SetHighlight(true);
			}
		}
	}
}

void JG_Game_Main::ccTouchesBegan(CCSet* pTouches, CCEvent* event)
{
	CCSetIterator i;
	CCTouch* touch;
	CCPoint tap;

	for( i = pTouches->begin(); i != pTouches->end(); i++) 
	{
		
		touch = (CCTouch*) (*i);
		tempDestination->setPosition(touch->getLocation());
		tempEnemy->SetDestination(tempDestination->getPosition(),NULL);
		if(touch) 
		{
			BallTouchHandler_Init(touch);
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
	//gameHUD->handdepict->setString(CCString::createWithFormat("%d",ballCounter)->getCString());
	//gameHUD->prevballcount->setString(CCString::createWithFormat("%d",prevballCounter)->getCString());
	prevballCounter = ballCounter;

}

/* This function first iterate through touches to find with wich hand they are colliding.
Then for each hand that is touch, finds wich ball is colliding with it.
*/
//TODO: clean up 
void JG_Game_Main::BallTouchHandler_Init(CCTouch* touch)
{
	CCPoint tap = touch->getLocation();
	JG_Hand* currentHand;
	JG_Ball* criticalBall;
	
	for(int j = 0 ; j< handsArray->count();j++)
	{
		currentHand = (JG_Hand*)handsArray->objectAtIndex(j);
		//Checking if tap is colliding with any of hands
		if(ArePointsColliding(tap,currentHand->getPosition(),currentHand->GetRadius()))
		{
			criticalBall = FindBestBallMatching(currentHand);
			if(criticalBall != NULL)	
			{

				SetTouchInfo(touch,currentHand,criticalBall);
			}

			//gameHUD->handdepict->setString(CCString::createWithFormat("%d",ballCounter)->getCString());
		}// end of hand collision checking
	}// end of hand looping	

}


JG_Ball* JG_Game_Main::FindBestBallMatching(JG_Hand * currentHand )
{
	float criticalTime;

	ballCounter = 0;

	criticalTime = 9999999999.0;
	JG_Ball* tempBall;
	JG_Ball* criticalBall = NULL ;
	//find the most critical ball (if exists) colliding with the ball
	//most critical ball is the ball witch will be lost befor other balls
	for (int k=0 ; k<ballsArray->count() ; k++)
	{
		tempBall=(JG_Ball *) ballsArray->objectAtIndex(k);
		if(ArePointsColliding(tempBall->getPosition(),currentHand->getPosition(),currentHand->GetRadius()+tempBall->radius))
		{
			if(currentHand==leftHand)
			{
				if(tempBall->GetBallDirection() == Dir_RightHandToUp  )
				{
					
					ballCounter++;
					//if(ballCounter == 1);
						//gameHUD->debugLabel->setString(CCString::createWithFormat("%f",absf(tempBall->getPositionY()/ tempBall->GetCurrentSpeedY()))->getCString());
					//if(ballCounter == 2);
						//gameHUD->balldepict->setString(CCString::createWithFormat("%f",absf(tempBall->getPositionY()/ tempBall->GetCurrentSpeedY()))->getCString());
					if( absf(tempBall->getPositionY()/ tempBall->GetCurrentSpeedY()) <criticalTime  )
					{
						criticalBall = tempBall;
						criticalTime = (tempBall->getPositionY()/ tempBall->GetCurrentSpeedY()) ;
					}
				}
				//TODO ayyoob : implement this
				else if (tempBall->GetBallDirection() == Dir_RighHandtToLeft)
				{
				}
			}
			else // if rightHand
			{


				if(tempBall->GetBallDirection() == Dir_LeftHandToRight)
				{
					ballCounter++;
					//if(ballCounter == 1);
						//gameHUD->debugLabel->setString(CCString::createWithFormat("%f",absf( tempBall->getPositionX()))->getCString());
					//if(ballCounter == 2);
						//gameHUD->balldepict->setString(CCString::createWithFormat("%f",absf( tempBall->getPositionX()))->getCString());

					if( ( abs(tempBall->getPositionX())) <criticalTime)
					{
						CCLog("it's all in your minde, tap location ");

						criticalBall = tempBall;
						criticalTime = absf((screenSize.width - tempBall->getPositionX())/tempBall->GetCurrentSpeedX());
					}
				}
				//TODO ayyoob : implement this
				else if( tempBall->GetBallDirection() == Dir_LeftHandToUp)
				{
				}

			}
		}// end of ball collision cheking
	}// end of ball looping

	return criticalBall;
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
		if (currentBall->GetBallDirection() != Dir_LeftHandToUp && currentBall->GetBallDirection()!=Dir_RightHandToUp)
		{
			if(currentHand==leftHand && currentBall->GetBallDirection()==Dir_RighHandtToLeft)
			{
				return true;
			}
			else if (currentHand==rightHand && currentBall->GetBallDirection()==Dir_LeftHandToRight)
			{
				return true;
			}
		}
	}
	//direction left
	else if(directionDeg>135&&directionDeg<225)
	{	
		if (currentBall->GetBallDirection() == Dir_LeftHandToUp)
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
		if (currentBall->GetBallDirection() == Dir_RightHandToUp)
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
	float throwPower = CalculateThrowPower(index);

	if(touchInfos[index].ball->GetMoveMode() == Move_Straight)
		touchInfos[index].hand->setThrowPower(throwPower);
	
	
	touchInfos[index].ball->Throw(throwPower,destHand->getPosition());

	//AddScore(touchInfos[index].ball->GetBallScore());
	//ManageBallComboScore(touchInfos[index].ball);

	ResetTouchInfo(index);
}

float JG_Game_Main::CalculateThrowPower(unsigned int index, bool bIsDemo)
{

	float touchLenght=abs(touchInfos[index].hand->getPositionY()-touchInfos[index].touch->getLocation().y);
	float currentRawPower=(touchLenght/maxTouchLenght)*maxThrowPower;

	return DiscretedPowerValueGen(currentRawPower,touchInfos[index].ball,bIsDemo);
}

float JG_Game_Main::DiscretedPowerValueGen(float rawInput,JG_Ball* ball, bool bIsDemo)
{
	rawInput = clampf(rawInput,actualMinPower,maxThrowPower);


	rawInput-=actualMinPower;
	//CCLOG("max value %f", GetMaxThrowPower());
	//CCLOG("min value %f", actualMinPower);
	//CCLOG("discrete value %f", (floor(rawInput/powerRange)*powerRange));
	float powerLevel=floor(rawInput/powerRange);


	discretedValue=powerLevel*powerRange;

	//CCLOG("power level : %f",powerLevel);

	// set ball level only when it is thrown up
	//if(!bIsDemo)
	//{
		//if(ball->GetBallDirection()== Dir_LeftHandToRight
		//	|| ball->GetBallDirection() ==Dir_RighHandtToLeft)
		//	ball->SetBallLevel(powerLevel);
	//}
	return discretedValue;

}

float JG_Game_Main::GetActualMinPower()
{
	return actualMinPower;
}

float JG_Game_Main::GetMaxThrowPower()
{
	return maxThrowPower;
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



void JG_Game_Main::ManageBallComboScore(JG_Ball * ball)
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

void JG_Game_Main::ManageFruitScore(JG_Fruit* fruit, JG_Ball* ball)
{

	AddScore( ball->IncrementAndGetComboChain() *fruit->GetScore());

	JG_ScorePopup * fruitScore = JG_ScorePopup::CreateScorePopup(this 
		, fruit->GetScore() 
		, ball->GetComboChain()
		, fruit->getPosition());
	
}

void JG_Game_Main::ManagePathScore(JG_Path* path)
{
	AddScore(path->GetScore());
}

void JG_Game_Main::OnBallsCollide(JG_Ball* ballOne,JG_Ball* ballTwo)
{
	//RemoveBallFromScreen(ballOne);
	//AddBallToScreen();
}

//collision of the ball and fruit
void JG_Game_Main::OnFruitHit(JG_Fruit* fruit, JG_Ball* ball)
{
	ManageFruitScore(fruit,ball);
	RemoveFruitFromScreen(fruit);
}

void JG_Game_Main::OnBallLost(JG_Ball* ball)
{
	//CCLog("BallLOst",0);
	int touchInfoIndex = GetTouchInfoIndexByBall(ball);
	if(touchInfoIndex!= -1)
		BallTouchHandler_End(touchInfoIndex);
	else
	{
		RemoveBallFromScreen(ball);
		DecrementLifeCount();
		if(lifeCount>0)
		{
			//AddBallToScreen();

		}
	}
}

void JG_Game_Main::OnFruitLost(JG_Fruit* fruit)
{
	RemoveFruitFromScreen(fruit);
}

void JG_Game_Main::OnBallThrow(JG_Ball* ball)
{
	ball->ResetComboChain();
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



void JG_Game_Main::IncrementReservedBallCount()
{
	reservedBallCount++;
}

void JG_Game_Main::DecrementReservedBallCount()
{
	reservedBallCount--;
}

void JG_Game_Main::SetReservedBallCount( int newCount)
{
	reservedBallCount = newCount;
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

void JG_Game_Main::RemoveAllFruitsFromScreen()
{
	JG_Fruit* tempFruit;
	int temp = fruitsArray->count();

	while(fruitsArray->count()>0)
	{
		RemoveFruitFromScreen((JG_Fruit*)fruitsArray->randomObject());
	}
}

void JG_Game_Main::RemoveFruitFromScreen(JG_Fruit* fruit)
{
	fruitsArray->removeObject(fruit,false);
	removeChild(fruit,true);
	CC_SAFE_RELEASE(fruit);
}

void JG_Game_Main::ReleaseBall(CCObject* pSender)
{
	if(reservedBallCount>0)
	{
		AddBallToScreen();
		DecrementReservedBallCount();
	}
}

void JG_Game_Main::AddBallToScreen()
{
	JG_Ball * newBall = JG_Ball::CreateBall(this
		,ccp(leftHand->getPositionX()
		,screenSize.height * 0.5 + CCRANDOM_0_1() *screenSize.height * 0.5)
		,Dir_RightHandToUp
		,DISCRETE_PARTS_COUNT);
	this->addChild(newBall,6);
	ballsArray->addObject(newBall);

}

void JG_Game_Main::TempAddBall(float dt)
{
	AddBallToScreen();

}

void JG_Game_Main::AddFruitToScreen()
{
	float tempX=CCRANDOM_0_1()*rightHand->getPosition().getDistance(leftHand->getPosition())+leftHand->getPositionX();
	float tempY=screenSize.height;
	CCPoint tempPoint;
	tempPoint.x=tempX;
	tempPoint.y=tempY;
	JG_Fruit* newFruit = JG_Fruit::CreateFruit(this,tempPoint,(-1)*(CCRANDOM_0_1()*10+15));
	
	this->addChild(newFruit);
	fruitsArray->addObject(newFruit);
	
}

void JG_Game_Main::TempAddFruitToScreen(float time)
{

	AddFruitToScreen();
	this->unschedule(schedule_selector(JG_Game_Main::TempAddFruitToScreen));
	this->schedule(schedule_selector(JG_Game_Main::TempAddFruitToScreen),CCRANDOM_0_1()*5);
}


void JG_Game_Main::UpdateHandPowerBar()
{
	((JG_GUI_Bar*)handsPowerBarArray->objectAtIndex(0))->SetBarScale(0);
	((JG_GUI_Bar*)handsPowerBarArray->objectAtIndex(1))->SetBarScale(0);
	for (int i=0;i<TOUCH_COUNT;i++)
	{
		if(touchInfos[i].touch!=NULL)
		{
			if(handsArray->objectAtIndex(0)==touchInfos[i].hand)
				((JG_GUI_Bar*)handsPowerBarArray->objectAtIndex(0))->SetBarScale((CalculateThrowPower(i)+actualMinPower)*2);
			else
				((JG_GUI_Bar*)handsPowerBarArray->objectAtIndex(1))->SetBarScale((CalculateThrowPower(i)+actualMinPower)*2);
		}

	}
}

void JG_Game_Main::UpdateBallThrowTrace()
{
	for (int i=0;i<TOUCH_COUNT;i++)
	{
		if(touchInfos[i].touch!=NULL && touchInfos[i].bIsDirValid
			&& touchInfos[i].ball->moveMode==Move_Straight)
		{
			//CCLog("WTTTTTTTTTTTTTTTF");
			if(touchInfos[i].hand== rightHand)
				touchInfos[i].ball->SetThrowPathInfo(CalculateThrowPower(i),rightHand->getPosition(),leftHand->getPosition());
			else
				touchInfos[i].ball->SetThrowPathInfo(CalculateThrowPower(i),leftHand->getPosition(),rightHand->getPosition());

		}

	}

}


void JG_Game_Main::InitialThrowPowerVariables()
{

	maxThrowPower=(JG_Ball::GetMaxSpeed()/JG_Ball::GetMinSpeed()-1);
	maxThrowPower*=(1+MIN_TOUCH_LENGTH_FACTOR);
	maxTouchLenght=THROW_FORCE_BASE_ON_SREEN*screenSize.height-rightHand->getPositionY();

	//min power that we can have
	actualMinPower=maxThrowPower*MIN_TOUCH_LENGTH_FACTOR;
	//range of power between max and min
	powerRange=(maxThrowPower-actualMinPower)/(DISCRETE_PARTS_COUNT-1);

	//CCLOG("Max maxThrowPower Length %f",maxThrowPower);
}


void JG_Game_Main::draw()
{
	//if(bIsGameInited)
		//DrawThrowPaths();
}

void JG_Game_Main::DrawThrowPaths()
{
	float power;
	for(int i = 0 ; i<DISCRETE_PARTS_COUNT; i++)
	{
		power = i*powerRange;
		DrawThrowPathByPower(power,checkCurvesLife(power));


	}
}

//TODO: clean up this shit
void JG_Game_Main::DrawThrowPathByPower(float _power, bool mustHighlight)
{
	
	//CCLOG(CCString::createWithFormat("chosen:%f",_power)->getCString());
	float tempSpeed =  JG_Ball::minSpeed + JG_Ball::minSpeed * _power;

	float tempSpeedX,tempSpeedY;
	CCPoint tracePoint;

	float tempCurveRad;
	tempCurveRad = JG_Ball::CalculateCurveRad(tempSpeed,rightHand->getPosition(),leftHand->getPosition());


	tracePoint = rightHand->getPosition();
	tempSpeedX= tempSpeed * cos(tempCurveRad);
	tempSpeedY = tempSpeed * sin(tempCurveRad);
	float tempInterval = 0.07;
	//for( int i = 0 ; i< 50 ; i++)
	while(tracePoint.y >= rightHand->getPositionY())
	{
		tempSpeedY = -GRAVITY* tempInterval  + tempSpeedY;
		//tempSpeedX = tempSpeedX;
		tracePoint.x = tempSpeedX * tempInterval + tracePoint.x;
		tracePoint.y = tempSpeedY * tempInterval + tracePoint.y;

		if(mustHighlight)
			traceLivePointTexture->drawAtPoint(convertToNodeSpace(tracePoint));
		else
			tracePointTexture->drawAtPoint(convertToNodeSpace(tracePoint));

		//tracePointTexture->SetOr
	}


}



bool JG_Game_Main::checkCurvesLife(float _power)
{
	
	//TODO ayyoob : find a better name
	//TODO ayyoob : implement this for reverse ball throwing ( from right hand to left)
	
	for (int i=0;i<TOUCH_COUNT;i++)
	{
		if(touchInfos[i].touch!=NULL && touchInfos[i].bIsDirValid
			&& touchInfos[i].ball->moveMode==Move_Straight)
		{
			
			if(CalculateThrowPower(i,true) == _power  && touchInfos[i].hand== rightHand)
				return true;
		}

	}

	JG_Hand * hand;
	for (int i=0 ; i<handsArray->count() ; i++)
	{
		hand = (JG_Hand* ) handsArray->objectAtIndex(i);

		if(hand->getThrowPower() == _power)
			return true;
	}
	return false;
}


void JG_Game_Main:: CheckBallCollisionWithHand()
{

	JG_Ball *currentBall;
	JG_Hand * currentHand;
	
	for(int i=0 ; i<handsArray->count() ; i++)
	{
		( (JG_Hand*) (handsArray->objectAtIndex(i)) ) ->SetAreaVisibility(false);
	}

	for(int i=0; i<ballsArray->count() ; i++)
	{
		currentBall = (JG_Ball * ) ballsArray->objectAtIndex(i);
		currentBall->SetShineVisibility(false);

		for (int j=0 ; j < handsArray->count() ; j++)
		{
			currentHand = (JG_Hand*) handsArray->objectAtIndex(j);
			
			if(ArePointsColliding(currentBall->getPosition() ,currentHand->getPosition() , currentHand->GetRadius()))
			{
				currentHand->SetAreaVisibility(true);
				currentBall->SetShineVisibility(true);			
			}
		}
	}
}


CCArray* JG_Game_Main::GetBallArray()
{
	return this->ballsArray;
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
	RemoveAllFruitsFromScreen();
	InitGame();
	ResumeGame(pSender);
}

void JG_Game_Main::EndGame()
{

	//	CCTimer::timerWithTarget( this,SEL_CallFuncO(JG_Game_Main::RestartGame),2);
	//CCTimer::timerWithTarget(

	//ballsArray->autorelease();

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



bool JG_Game_Main::ArePointsColliding(CCPoint point1,CCPoint point2,float distance)
{
	return point1.getDistance(point2)<distance;
}


float JG_Game_Main::absf(float input)
{
	if(input > 0.0)
		return input;

	return 0.0 - input;

}

template<class enemyClass>
SEnemyTypes JG_Game_Main::CreateEnemyType(int baseChance,int chaceIncrease)
{
	SEnemyTypes enemyType;
	enemyType.factory = new JG_Factory_Enemy<enemyClass>;
	enemyType.currentChance = baseChance;
	enemyType.chanceIncreasePerRound = chaceIncrease;
	return enemyType;
}


int JG_Game_Main::getAttackWaveType()
{
	return CCRANDOM_0_1()*attackWaveTypes.size();
}

void JG_Game_Main::ManageDifficulty()
{
	//int attackWaveIndex = getAttackWaveType();

	//JG_AttackWave_Base* currentAttackWave;
	//currentAttackWave = (JG_AttackWave_Base*)  attackWaveTypes[attackWaveIndex]->Create();

	//float difficulty = 100*attackWaveCount ;

	//currentAttackWave->initAttacWave(this,difficulty,attackWaveCount);

	//attackWaveCount++;
}
