#include "JG_Game_Main.h"

#include "pthread/pthread.h"
#include "JG_SoundEngine.h"



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
	currentAttackWave = NULL;
	prevballCounter =0 ;
	
	if ( !CCLayer::init() )
	{
		return false;
	}

	screenSize = CCDirector::sharedDirector()->getWinSize();

	InitialLoadingScreen();

	this->setTouchEnabled(true);
	this->setKeypadEnabled(true);
	return true;
}

void JG_Game_Main::InitialLoadingScreen()
{
	CCSprite * loadScreen = CCSprite::create("LoadingScreen.png");
	loadScreen->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	ScaleSpriteToDesignSize(loadScreen);
	this->addChild(loadScreen,-100);

	this->schedule(schedule_selector(JG_Game_Main::InitialGameTimer),0.1,0,0.1);
}

void JG_Game_Main::InitialGameTimer(float dt)
{
	InitialGame();
}

void JG_Game_Main::InitialGame()
{
	

	this->schedule(schedule_selector(JG_Game_Main::update));

	InitialCallBacks();
	
	CreateGUI();
	CreateBackGround();
	CreateHands();

	InitialBallVariablesVariables();
	InitialPaths();
	InitialEnemies();

	ResumeGame();
	InitRound();
}


void JG_Game_Main::InitialEnemies()
{
	enemyTypes.push_back(CreateEnemyType<JG_Enemy_QuickAttakc>(3,3));
	enemyTypes.push_back(CreateEnemyType<JG_Enemy_Crow>(6,2));

	LoadEnemysContents();
}

void JG_Game_Main::LoadEnemysContents()
{
	JG_Enemy_Base * e;
	for(int i  = 0 ; i<enemyTypes.size(); i++)
	{
		e = (JG_Enemy_Base*) enemyTypes.at(i).factory->Create();
		e->InitialEnemy(ccp(screenSize.width-100,0));
	}
}

void JG_Game_Main::InitialPaths()
{
	JG_Path::InitialPathHealthStatesForEachLevel();

	pathsArray = CCArray::create();
	pathsArray->retain();
	for( int i= 0 ; i< DISCRETE_PARTS_COUNT ; i++)
	{
		pathsArray->addObject(JG_Path::CreatePath(this
			,i * powerRange
			, leftHand->getPosition()
			, rightHand->getPosition()));
		this->addChild((CCNode*)pathsArray->objectAtIndex(i),18);
	}
}

void JG_Game_Main::InitialCallBacks()
{
	JG_Enemy_Base::SetOnHitFunctionPointer(this,OnHitHandler(&JG_Game_Main::OnEnemyHit));
	JG_Enemy_Base::SetOnLostFunctionPointer(this,OnLostHandler(&JG_Game_Main::OnEnemyLost));
	JG_Enemy_Base::SetGetBallsFunctionPointer(this,GetBallsHandler(&JG_Game_Main::GetBallArray));
	JG_Enemy_Base::SetDamagePathFunctionPointer(this,DamagePathHandler(&JG_Game_Main::DamagePath));
	JG_Enemy_Base::SetGetBallRadiusFunctionPointer(this,GetBallRadiusHandler(&JG_Game_Main::GetBallRadius));
	JG_Enemy_Base::SetPlayMusicFunctionPointer(this,PlayMusicHandler(&JG_Game_Main::playSoundEffect));

	JG_AttackWave_Base::SetGetEnemyTypesFunctionPointer(this,GetEnemyTypesHandler(&JG_Game_Main::getEnemyTypes));
	JG_AttackWave_Base::SetGetBallsToRewardFunctionPointer(this,GetBallsToRewardCountHandler(&JG_Game_Main::getBallsToRewardCount));
	JG_AttackWave_Base::SetGetHealthesToRewardCountFunctionPointer(this,GetHealthesToRewardCountHandler(&JG_Game_Main::getHealthsToRewardCount));
	JG_AttackWave_Base::SetGetPathesArrayFunctionPointer(this,GetPathesArrayHandler(&JG_Game_Main::getpathsArray));
	JG_AttackWave_Base::SetOnAttackWaveFinishedFunctionPointer(this,OnAttackWaveFinishedHandler(&JG_Game_Main::onAttackWaveFinished));
	JG_AttackWave_Base::SetOnBallRewardedFunctionPointer(this,OnBallRewardedHandler(&JG_Game_Main::onBallRewarded));
	JG_AttackWave_Base::SetOnHealthRewardedFunctionPointer(this,OnHealthRewardedHandler(&JG_Game_Main::onHealthRewarded));
	JG_AttackWave_Base::SetAddEnemyFunctionPointer(this, AddEnemyHandler(&JG_Game_Main::addEnemy));
	JG_AttackWave_Base::SetGetAvailablePathCountFunctionPointer(this, GetAvailablePathCountHandler(&JG_Game_Main::getAvailablePathCount));
}

void JG_Game_Main::CreateGUI()
{
	gameGUI = JG_Game_GUI::create(this);
	gameGUI->retain();
	gameGUI->setPosition(CCPointZero);
	this->addChild(gameGUI,100);
}

void JG_Game_Main::CreateBackGround()
{
	CCSprite * backGround = CCSprite::create("background4.png");
	backGround->setPosition(ccp(screenSize.width/2,screenSize.height/2));
	ScaleSpriteToDesignSize(backGround);
	this->addChild(backGround,-100);
}

void JG_Game_Main::CreateHands()
{
	rightHand = JG_Hand::CreateHand(this,ccp(screenSize.width * 0.85 ,screenSize.height * 0.15),"RightHand.png", "touchLayer.png");
	leftHand = JG_Hand::CreateHand(this,ccp(screenSize.width * 0.15,screenSize.height * 0.15),"LeftHand.png", "touchLayer.png");

	handsArray= CCArray::create(rightHand,leftHand,NULL);
	handsArray->retain();

	for( int i = 0 ; i<handsArray->count();i++)
		this->addChild((CCNode*)handsArray->objectAtIndex(i),5);

}


void JG_Game_Main::InitGame_difficultyControl()
{
	gameGUI->HideGUIScreens();
	gameGUI->SetHUDVisibility(true);
	ballsToRewardCounter = 0 ;
	totalBallsRewarded = 0 ;
	
	totalhealthsRewarded = 0;
	
	healthsToRewardCounter = 0;

	initialTotalHealth = DISCRETE_PARTS_COUNT * 100;

	attackWaveTypes.clear();

	enemyArray = CCArray::create();
	enemyArray->retain();

	attackWaveTypes.push_back(new JG_Factory_AttackWave<JG_AttackWave_AllLinesSequential>);
	attackWaveTypes.push_back(new JG_Factory_AttackWave<JG_AttackWave_AllLinesTogether>);

	attackWaveCount = 0;

	ManageDifficulty();
}

void JG_Game_Main::InitRound()
{
	JG_SoundEngine::playMusic("BackMusic.wav");
	scoreTable = scoreFileHandler->GetHighScoreTable();
	gameGUI->SetHUDVisibility(true);
	tracePointTexture = CCTextureCache::sharedTextureCache()->addImage("deadStar.png");
	traceLivePointTexture = CCTextureCache::sharedTextureCache()->addImage("liveStar.png");

	CreateBalls();

	for( int i = 0 ; i< TOUCH_COUNT ; i++)
	{
		touchInfos[i].ball = NULL;
		touchInfos[i].hand = NULL;
		touchInfos[i].touch = NULL;
		touchInfos[i].bIsDirValid = false;
	}

	InitGame_difficultyControl();
	
	SetLifeCount(MAX_LIFE_COUNT);
	SetScore(0);
	SetReservedBallCount(INIT_BALL_COUNT);
	bIsGameInited = true;

}

void JG_Game_Main::CreateBalls()
{
	ballsArray=CCArray::create();
	ballsArray->retain();

	fruitsArray= CCArray::create();
	fruitsArray->retain();

	for (int i=0;i<pathsArray->count();i++)
	{
		((JG_Path* )pathsArray->objectAtIndex(i))->ResetPath();
	}
}




void JG_Game_Main::update(float dt)
{
	CheckBallsThrowPath();
	BallTouchHandler_CheckTime(dt);
	//UpdateHandPowerBar();
	//UpdateBallThrowTrace(); 

	//TestSingleTouch();
	CheckBallCollisionWithHand();
	

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
			if(touchInfos[i].bIsDirValid)
			{
				if(touchInfos[i].ball->GetBallDirection() == Dir_RighHandtToLeft
					|| touchInfos[i].ball->GetBallDirection() == Dir_LeftHandToRight)
				{
					int ballPath = CalculateThrowPower(i,true)/ powerRange;

					((JG_Path * ) pathsArray->objectAtIndex(ballPath))->SetHighlight(true);
				}
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
		//tempDestination->setPosition(touch->getLocation());
		if(tempEnemy!=NULL)
		{
			//tempEnemy->SetDestinationPath(tempDestination->getPosition(),NULL);
		}
		if(touch) 
		{
			touchBeginHandler(touch);

			/*********** Sorry for this shit *******************/
			if(gameGUI->IsPlayerNameTextBoxVisible())
			{
				gameGUI->CheckPlayerNameTextBoxTouched(touch);
			}
			/*************************************************/

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

		touchEndHandler(touch);
	}
	//gameGUI->handdepict->setString(CCString::createWithFormat("%d",ballCounter)->getCString());
	//gameGUI->prevballcount->setString(CCString::createWithFormat("%d",prevballCounter)->getCString());
	prevballCounter = ballCounter;

}

void JG_Game_Main::touchBeginHandler(CCTouch *touch)
{
	JG_Hand* currentHand;
	CCPoint tap = touch->getLocation();

	for(int j = 0 ; j< handsArray->count();j++)
	{
		currentHand = (JG_Hand*)handsArray->objectAtIndex(j);
		//Checking if tap is colliding with any of hands
		if(ArePointsColliding(tap,currentHand->getPosition(),currentHand->GetRadius()))
		{
			currentHand->SetTouchLayerVisible(true);
			BallTouchHandler_Init(currentHand, touch);
		}
	}
}

void JG_Game_Main::touchEndHandler(CCTouch* touch)
{
	JG_Hand* currentHand;
	CCPoint tap = touch->getStartLocation();

	for(int j = 0 ; j< handsArray->count();j++)
	{
		currentHand = (JG_Hand*)handsArray->objectAtIndex(j);
		//Checking if tap is colliding with any of hands
		if(ArePointsColliding(tap,currentHand->getPosition(),currentHand->GetRadius()))
		{
			currentHand->SetTouchLayerVisible(false);
			
		}
	}
}

/* This function first iterate through touches to find with wich hand they are colliding.
Then for each hand that is touch, finds wich ball is colliding with it.
*/
//TODO: clean up 
void JG_Game_Main::BallTouchHandler_Init(JG_Hand *hand, CCTouch *touch)
{
	
	
	JG_Ball* criticalBall;

	
	criticalBall = FindBestBallMatching(hand);
	if(criticalBall != NULL)	
	{

		SetTouchInfo(touch,hand,criticalBall);
	}

			//gameGUI->handdepict->setString(CCString::createWithFormat("%d",ballCounter)->getCString());
		

}


JG_Ball* JG_Game_Main::FindBestBallMatching(JG_Hand * currentHand )
{
	float criticalTime;

	ballCounter = 0;

	criticalTime = 9999999999.0;
	JG_Ball* tempBall;
	JG_Ball* criticalBall = NULL ;
	//find the most critical ball (if exists) colliding with the ball
	//most critical ball is the ball witch will be lost before other balls
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
					//gameGUI->debugLabel->setString(CCString::createWithFormat("%f",JG_abs(tempBall->getPositionY()/ tempBall->GetCurrentSpeedY()))->getCString());
					//if(ballCounter == 2);
					//gameGUI->balldepict->setString(CCString::createWithFormat("%f",JG_abs(tempBall->getPositionY()/ tempBall->GetCurrentSpeedY()))->getCString());
					if( JG_abs(tempBall->getPositionY()/ tempBall->GetCurrentSpeedY()) <criticalTime  )
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
					if( ( JG_abs(tempBall->getPositionX())) <criticalTime)
					{
						criticalBall = tempBall;
						criticalTime = JG_abs((screenSize.width - tempBall->getPositionX())/tempBall->GetCurrentSpeedX());
					}
				}
				//TODO ayyoob : implement this
				else if( tempBall->GetBallDirection() == Dir_LeftHandToUp)
				{
				}

			}
		}// end of ball collision checking
	}// end of ball looping

	return criticalBall;
}


void JG_Game_Main::BallTouchHandler_CheckDirection(unsigned int index)
{
	if(touchInfos[index].ball == NULL)
		return;
	if(!touchInfos[index].bIsDirValid)
	{
		touchInfos[index].bIsDirValid = SetTouchDirectionForBall(index);
	}
	else
	{

		//bool tempDirIsValid = ;
		if(!SetTouchDirectionForBall(index))
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
	CheckBallsThrowPath();

	//AddScore(touchInfos[index].ball->GetBallScore());
	//ManageBallComboScore(touchInfos[index].ball);

	ResetTouchInfo(index);
}

float JG_Game_Main::CalculateThrowPower(unsigned int index, bool bIsDemo)
{
	float touchLenght=JG_abs(touchInfos[index].hand->getPositionY()-touchInfos[index].touch->getLocation().y);
	float currentRawPower=(touchLenght/maxTouchLenght)*maxThrowPower;

	return DiscretedPowerValueGen(currentRawPower,touchInfos[index].ball,bIsDemo);
}

float JG_Game_Main::DiscretedPowerValueGen(float rawInput,JG_Ball* ball, bool bIsDemo)
{
	rawInput = clampf(rawInput,actualMinPower,maxThrowPower);

	rawInput-=actualMinPower;

	float powerLevel=floor(rawInput/powerRange);

	discretedValue=powerLevel*powerRange;
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

void JG_Game_Main::ManagePathScore(JG_Path* path)
{
	AddScore(path->GetScore());
	JG_ScorePopup::CreateScorePopup(this,path->GetScore(),1,path->GetPositionForLengthRatio(0.7));
}


void JG_Game_Main::ManageEnemyBonus(EEnemyBonus bonus)
{
	switch (bonus)
	{
	case EnemyBonus_None:
		break;
	case EnemyBonus_ExtraBall:
		IncrementReservedBallCount();
		break;
	case EnemyBonus_PathHealth:
		HealPath();
		break;
	default:
		break;
	}

}

void JG_Game_Main::OnBallsCollide(JG_Ball* ballOne,JG_Ball* ballTwo)
{
	//RemoveBallFromScreen(ballOne);
	//AddBallToScreen();
}

void JG_Game_Main::OnEnemyHit(JG_Enemy_Base* enemy, JG_Ball* ball)
{
	//maybe score for hitting enemy
	//when a ball hit the enemy it goes for state dying
	//CCLog("!!!!OnEnemyHit!!!!!!");
	enemy->SetState(EnemyS_Dying);
	ManageEnemyBonus(enemy->GetEnemyBonus());
	JG_SoundEngine::playSoundEffect("hitSound.wav");

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
		//DecrementLifeCount();
		if(lifeCount>0)
		{
			//AddBallToScreen();

		}
	}
	CheckLoseCondition();
}


void JG_Game_Main::OnPathLost(JG_Path* path)
{
	JG_Enemy_Base* currentEnemy;
	for(int i=0;i<enemyArray->count();i++)
	{
		currentEnemy = (JG_Enemy_Base*)enemyArray->objectAtIndex(i);
		if(currentEnemy->GetTargetPath() == path)
			currentEnemy->SetState(EnemyS_Escaping);
	}
	RemovePathFromScreen(path);
	CheckLoseCondition();
	
	
}
void JG_Game_Main::OnEnemyLost(JG_Enemy_Base* enemy)
{
	
	
	RemoveEnemyFromScreen(enemy);
	ManageDifficulty();
}

void JG_Game_Main::OnBallThrow(JG_Ball* ball)
{
	JG_SoundEngine::playSoundEffect("Throwing.wav");
}

void JG_Game_Main::DamagePath(JG_Path* path,float damage)
{
	path->TakeDamage(damage);
}

float JG_Game_Main::GetBallRadius(JG_Ball* ball)
{
	return ball->radius;
}

bool JG_Game_Main::IsThereAnyBallLeft()
{
	return (reservedBallCount >0 || ballsArray->count()>0);
}

bool JG_Game_Main::IsThereAnyPathLeft()
{
	JG_Path* currentPath;
	for(int i = 0 ; i< pathsArray->count(); i++)
	{
		currentPath = (JG_Path*) pathsArray->objectAtIndex(i);
		if(currentPath->IsPathEnabled())
			return true;
	}
	return false;
}

int JG_Game_Main::GetScore()
{
	return playerScore;
}

void JG_Game_Main::SetScore( int newScore)
{
	playerScore = newScore;
	gameGUI->SetPlayerScore(playerScore);

}

void JG_Game_Main::AddScore(int amount)
{

	playerScore+= amount;
	gameGUI->SetPlayerScore(playerScore);

}

void JG_Game_Main::ReduceScore(int amount)
{
	playerScore-= amount;
	gameGUI->SetPlayerScore(playerScore);
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
		EndRound();
}

void JG_Game_Main::IncrementLifeCount()
{
	++lifeCount;
}



void JG_Game_Main::IncrementReservedBallCount()
{
	reservedBallCount++;
	gameGUI->SetPlayerReservedBall(reservedBallCount);
}

void JG_Game_Main::DecrementReservedBallCount()
{
	reservedBallCount--;
	gameGUI->SetPlayerReservedBall(reservedBallCount);
}

void JG_Game_Main::SetReservedBallCount( int newCount)
{
	reservedBallCount = newCount;
	gameGUI->SetPlayerReservedBall(reservedBallCount);
}

void JG_Game_Main::HealPath()
{
	JG_Path * desiredPath;

	desiredPath = FindADestroyedPath();

	if(desiredPath==NULL)
		desiredPath= FindMostDamagedPath();

	desiredPath->ResetPath();
}

JG_Path* JG_Game_Main::FindADestroyedPath()
{
	JG_Path* path;
	for(int i= 0 ; i<pathsArray->count();i++)
	{
		path = (JG_Path*)pathsArray->objectAtIndex(i);
		if( !path->IsPathEnabled() )
			return path;
	}
	return NULL;
}

JG_Path* JG_Game_Main::FindMostDamagedPath()
{
	JG_Path* path;
	JG_Path* mostDamaged=NULL;
	for(int i= 0 ; i<pathsArray->count();i++)
	{
		path = (JG_Path*)pathsArray->objectAtIndex(i);
		if( mostDamaged==NULL || mostDamaged->GetHealth()> path->GetHealth() )
			mostDamaged = path;
	}
	return mostDamaged;

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


void JG_Game_Main::RemovePathFromScreen(JG_Path* path)
{
	path->SetPathEnable(false);
	//pathsArray->removeObject(path,false);
	//removeChild(path,true);
	//CC_SAFE_RELEASE(path);
}

void JG_Game_Main::RemoveEnemyFromScreen(JG_Enemy_Base* enemy)
{

	enemyArray->removeObject(enemy,false);
	removeChild(enemy,true);
	CC_SAFE_RELEASE(enemy);
	//onenemylost
	tempEnemy=NULL;
}

void JG_Game_Main::RemoveAllEnemiesFromScreen()
{
	JG_Enemy_Base* tempEnemy;


	while(enemyArray->count()>0)
	{
		RemoveEnemyFromScreen((JG_Enemy_Base*)enemyArray->randomObject());
	}

}

void JG_Game_Main::ReleaseBall(CCObject* pSender)
{
	if(reservedBallCount>0)
	{
		AddBallToScreen();
		JG_SoundEngine::playSoundEffect("releaseball.wav");
		DecrementReservedBallCount();
	}
}

void JG_Game_Main::AddBallToScreen()
{
	JG_Ball * newBall = JG_Ball::CreateBall(this
		,ccp(leftHand->getPositionX()
		,screenSize.height * 0.8)
		,Dir_RightHandToUp
		,DISCRETE_PARTS_COUNT);
	this->addChild(newBall,100);
	ballsArray->addObject(newBall);

}

void JG_Game_Main::TempAddBall(float dt)
{
	AddBallToScreen();

}

void JG_Game_Main::InitialBallVariablesVariables()
{
	JG_Ball::CalculateSpeedBoundriesBaseOnLength(rightHand->getPositionX()-leftHand->getPositionX());


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

	/*for(int i=0 ; i<handsArray->count() ; i++)
	{
		( (JG_Hand*) (handsArray->objectAtIndex(i)) ) ->SetAreaVisibility(false);
	}*/

	for(int i=0; i<ballsArray->count() ; i++)
	{
		currentBall = (JG_Ball * ) ballsArray->objectAtIndex(i);
		currentBall->SetShineVisibility(false);

		for (int j=0 ; j < handsArray->count() ; j++)
		{
			currentHand = (JG_Hand*) handsArray->objectAtIndex(j);

			if(ArePointsColliding(currentBall->getPosition() ,currentHand->getPosition() , currentHand->GetRadius()))
			{
				//currentHand->SetAreaVisibility(true);
				currentBall->SetShineVisibility(true);			
			}
		}
	}
}


int JG_Game_Main::GetPathLevelByPower(float pathPower)
{
	return pathPower/powerRange;
}
CCArray* JG_Game_Main::GetBallArray()
{
	return this->ballsArray;
}


void JG_Game_Main::PauseGame()
{
	
	CCDirector::sharedDirector()->pause();
}

void JG_Game_Main::ExitToMainMenu()
{
	CCDirector::sharedDirector()->replaceScene(JG_Menu_Main::scene());
}


void JG_Game_Main::ExitGame()
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

void JG_Game_Main::ResumeGame()
{
	CCDirector::sharedDirector()->resume();
	gameGUI->SetPauseScreenVisibility(false);
}

void JG_Game_Main::ResetGame()
{
	RemoveAllBallsFromScreen();
	RemoveAllEnemiesFromScreen();
	//RemoveAllEnemiesFromScreen();
	InitRound();
	ResumeGame();
	resetDifficulty();

}

void JG_Game_Main::HandlePauseGame(CCObject* pSender)
{
	gameGUI->SetPauseScreenVisibility(true);
	PauseGame();
	JG_SoundEngine::pauseMusic();
	JG_SoundEngine::playSoundEffect("EffectSound.wav");
}

void JG_Game_Main::HandleExitGame(CCObject* pSender)
{
	ExitGame();
}

void JG_Game_Main::HandleExitToMainMenu(CCObject* pSender)
{
	ExitToMainMenu();
	ResumeGame();
	

}

void JG_Game_Main::HandleResumeGame(CCObject* pSender)
{
	ResumeGame();
	JG_SoundEngine::playSoundEffect("EffectSound.wav");
	JG_SoundEngine::resumeMusic();
}

void JG_Game_Main::HandleResetGame(CCObject* pSender)
{
	ResetGame();
	JG_SoundEngine::playSoundEffect("EffectSound.wav");
	JG_SoundEngine::resumeMusic();
}

void JG_Game_Main::HandleEndRoundScreenResetGame(CCObject* pSender)
{
	InsertPlayerHighScore(gameGUI->GetPlayerName()
		,rank
		,playerScore);
	ResetGame();
}

void JG_Game_Main::HandleEndRoundScreenExitToMainMenu(CCObject* pSender)
{
	InsertPlayerHighScore(gameGUI->GetPlayerName()
		,rank
		,playerScore);
	ExitToMainMenu();
}




void JG_Game_Main::EndRound()
{
	ScoreTableRecord firstRank = scoreTable->at(0);
	gameGUI->SetEndRoundScreenInfos(playerScore,firstRank.score,firstRank.name);
	gameGUI->SetEndRoundScreenVisibility(true);
	if(IsPlayerGetHighScore())
	{
		rank = DeterminePlayerRank();
		gameGUI->SetHighScoreScreenInfos(rank);
		gameGUI->SetHighScoreScreenVisibility(true);
	}
	
	PauseGame();
}

bool JG_Game_Main::IsPlayerGetHighScore()
{
	//TODO: this code is not comprehensible
	auto leastScore = scoreTable->at(scoreTable->size()-1).score;
	return playerScore >= leastScore;
}

int JG_Game_Main::DeterminePlayerRank()
{
	for(int i = 0 ; i < scoreTable->size(); i++)
	{
		if( playerScore >= scoreTable->at(i).score)
			return i+1;
	}
	return -1;
}

void JG_Game_Main::InsertPlayerHighScore(CCString _playerName, int _rank, int _score)
{
	//extra check
	if(IsPlayerGetHighScore())
	{
		for( int i = _rank ; i < scoreTable->size(); i++)
		{
			scoreFileHandler->InsertRecord(scoreTable->at(i).name
				,scoreTable->at(i).score
				,scoreTable->at(i).rank+1);
		}
		scoreFileHandler->InsertRecord(_playerName, _score,_rank);
	}
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


void JG_Game_Main::CheckLoseCondition()
{
	if(!IsThereAnyBallLeft() || !IsThereAnyPathLeft())
	{
		EndRound();
	}
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








template<class enemyClass>
SEnemyTypes JG_Game_Main::CreateEnemyType(int baseChance,int chaceIncrease)
{
	SEnemyTypes enemyType;
	enemyType.factory = new JG_Factory_Enemy<enemyClass>;
	enemyType.currentChance = baseChance;
	enemyType.chanceIncreasePerRound = chaceIncrease;
	return enemyType;
}

//difficulty control and 
int JG_Game_Main::getAttackWaveType()
{
	return CCRANDOM_0_1()*attackWaveTypes.size();
}

void JG_Game_Main::initialNewAttackWave(float dt)
{


	CCLOG("called initial new attackwave ");
	int attackWaveIndex = getAttackWaveType();
	//CCLOG(CCString::createWithFormat("attackwaveindex: %d" , attackWaveIndex)->getCString());
	
	currentAttackWave = (JG_AttackWave_Base*)  attackWaveTypes[attackWaveIndex]->Create();
	currentAttackWave->retain();
	addChild(currentAttackWave);
	//currentAttackWave = (JG_AttackWave_Base*) new JG_AttackWave_AllLinesSequential();
	float difficulty = 100 * (attackWaveCount+1) ;
	//CCLOG(CCString::createWithFormat("difficulty: %f" , difficulty)->getCString());
	currentAttackWave->initAttackWave(difficulty, (attackWaveCount+1) );

	attackWaveCount++;
	CCLOG("finished initial new attack wave");
}

void JG_Game_Main::ManageDifficulty()
{
	if(enemyArray->count() == 0)
	{
		unschedule(schedule_selector(JG_Game_Main::initialNewAttackWave));
		schedule(schedule_selector(JG_Game_Main::initialNewAttackWave),0,0,3);
	}

	if(attackWaveCount == 0)
	{
		schedule(schedule_selector(JG_Game_Main::manageRewards),5);
	}

}

void JG_Game_Main::onAttackWaveFinished()
{
	//TODO ayoob: fix this
	if(currentAttackWave==NULL)
		return;

	removeChild(currentAttackWave,true);
	CC_SAFE_RELEASE(currentAttackWave);
	currentAttackWave = NULL;

	
}

int JG_Game_Main::getAvailablePathCount()
{
	int tempPathCounter=0;
	for(int i=0;i<pathsArray->count(); i++)
	{
		if( ( (JG_Path*)pathsArray->objectAtIndex(i) )->IsPathEnabled() )
		{
			tempPathCounter++;
		}
	}

	return tempPathCounter;
}

void JG_Game_Main::resetDifficulty()
{
	attackWaveCount= 0;
	ballsToRewardCounter = 0;
	healthsToRewardCounter = 0;
	totalBallsRewarded = 0;
	totalhealthsRewarded = 0;
	onAttackWaveFinished();

	ManageDifficulty();
}


void JG_Game_Main::manageRewards(float dt)
{
	//balls reward algorithm
	int a = INIT_BALL_COUNT - (ballsArray->count() + reservedBallCount);
	
	float lostBallCount =  a;
	if(lostBallCount < 1 )
	{
		lostBallCount = 0.5;
	}

	//float attackWave_count_effect = attackWaveCount / 4 ;
	float attackWaveCountEffect = (attackWaveCount+1) / 4;
	
	
	ballsToRewardCounter =  lostBallCount * ( attackWaveCountEffect * CCRANDOM_0_1() ) * 1/ (1+ totalBallsRewarded) ;
	if(ballsToRewardCounter >2)
	{
		ballsToRewardCounter =2;
	}
	totalBallsRewarded += ballsToRewardCounter;


	//health bonus algorithm

	float lostHealthFactor = calculateLostHealth() / 100;
	//CCLOG(CCString::createWithFormat("losthealth factor: %f\n attackwave effect: %f" , lostHealthFactor, attackWaveCountEffect)->getCString());
	if(lostHealthFactor <1)
	{
		lostHealthFactor = 0.5;
	}

	//healthsToRewardCounter = lostHealthFactor * (attackWaveCountEffect * CCRANDOM_0_1() ) * ( 1/ (1 + totalhealthsRewarded) );
	float chanceFactor = CCRANDOM_0_1();
	healthsToRewardCounter = lostHealthFactor * attackWaveCountEffect * chanceFactor * (1.0/(1+totalhealthsRewarded));
	
	if(healthsToRewardCounter > 2)
	{
		healthsToRewardCounter =2;
	}
	//CCLOG(CCString::createWithFormat(" totalhealthrewards:%i" , totalhealthsRewarded)->getCString());
	//CCLOG(CCString::createWithFormat("healthto be rewarded: %i" , healthsToRewardCounter)->getCString());
	//CCLOG(CCString::createWithFormat("healths to reward: %f\n totalhealthrewards:%i" , healthsToRewardCounter, totalhealthsRewarded)->getCString());
	totalhealthsRewarded += healthsToRewardCounter;
	
	
	//CCLOG(CCString::createWithFormat("to be added balls: %d", ballsToRewardCounter)->getCString());

}

void JG_Game_Main::onBallRewarded()
{
	
	
		ballsToRewardCounter -= 1;
	
}

int JG_Game_Main::getBallsToRewardCount()
{
	return ballsToRewardCounter;
}

float JG_Game_Main::calculateLostHealth()
{
	float currentTotalHealth=0;

	for(int i=0; i<DISCRETE_PARTS_COUNT; i++)
	{
		if( ((JG_Path*) pathsArray->objectAtIndex(i))->IsPathEnabled())
		{
			currentTotalHealth += ((JG_Path*) pathsArray->objectAtIndex(i))->GetHealth();
		}
	}
	
	return initialTotalHealth - currentTotalHealth;
}

std::vector<SEnemyTypes> JG_Game_Main::getEnemyTypes()
{
	return enemyTypes;
}

CCArray* JG_Game_Main::getpathsArray()
{
	return pathsArray;
}

void JG_Game_Main::addEnemy(JG_Enemy_Base* enemy)
{
	enemyArray->addObject(enemy);
	addChild((CCNode*) enemy,20);
}

/////////////////////////////////////////////////////////////////////
int JG_Game_Main::getHealthsToRewardCount()
{
	return healthsToRewardCounter;
}

void JG_Game_Main::onHealthRewarded()
{
	healthsToRewardCounter -= 1;
}

void JG_Game_Main::playSoundEffect(CCString effectsound)
{
	JG_SoundEngine::playSoundEffect(effectsound.getCString());
}


void JG_Game_Main::TestOutOfRangeRankForHighScore()
{
	InsertPlayerHighScore("",HIGH_SCORE_RECORD_NUMBERS+1,0);
	vector<ScoreTableRecord>* table = scoreFileHandler->GetHighScoreTable();

	for( int i =0 ; i<table->size();i++)
	{
		if(table->at(i).rank == HIGH_SCORE_RECORD_NUMBERS+1)
			exit(-1);
	}
	exit(0);
}








