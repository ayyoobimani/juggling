#include "JG_Main_Game.h"


USING_NS_CC;

CCScene* JG_Main_Game::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    JG_Main_Game *layer = JG_Main_Game::create();

    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool JG_Main_Game::init()
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
	this->schedule(schedule_selector(JG_Main_Game::update));


	/*********************** Background **************************/
	CCSprite * backGround = CCSprite::create("background.png");

	backGround->setPosition(ccp(screenSize.x/2,screenSize.y/2));
	this->addChild(backGround);
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
	ballsArray=CCArray::create(JG_Ball::CreateBall(ccp(leftHand->getPositionX(),200),EDir_RightHandToUp),NULL);
	ballsArray->retain();

	for( int i = 0 ; i<ballsArray->count();i++)
	{
		this->addChild((CCNode*)ballsArray->objectAtIndex(i),6);
	}

	/******************************** /Balls ************************************/


	currentBall = NULL;
	currentHand = NULL;
	bDirIsSet = false;
	


	this->setTouchEnabled(true);
    return true;
}


void JG_Main_Game::menuCloseCallback(CCObject* pSender)
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

void JG_Main_Game::menuPauseCallBack(CCObject* pSender)
{
	if(!CCDirector::sharedDirector()->isPaused())
		CCDirector::sharedDirector()->pause();
	else
		CCDirector::sharedDirector()->resume();
}

/* This function first iterate through touches to find with wich hand they are colliding.
	Then for each hand that is touch, finds wich ball is colliding with it.
*/
void JG_Main_Game::ccTouchesBegan(CCSet* pTouches, CCEvent* event)
{
	//TODO: implement this for multi touch
	CCSetIterator i;
	CCTouch* touch;
	CCPoint tap;

	for( i = pTouches->begin(); i != pTouches->end(); i++) 
	{
		touch = (CCTouch*) (*i);
		if(touch) 
		{
			JG_Hand * tempHand;
			tap = touch->getLocation();
			for(int j = 0 ; j< handsArray->count();j++)
			{
				tempHand = (JG_Hand*)handsArray->objectAtIndex(j);
				//Checking if tap is colliding with any of hands
				if(ArePointsColliding(tap,tempHand->getPosition(),tempHand->GetRadius()))
				{
					currentHand=((JG_Hand*)handsArray->objectAtIndex(j));
					JG_Ball *testBall;
					//cheking if hand is colliding with a ball
					for (int k=0 ; k<ballsArray->count() ; k++)
					{
						testBall=(JG_Ball *) ballsArray->objectAtIndex(k);
						if(ArePointsColliding(testBall->getPosition(),currentHand->getPosition(),currentHand->GetRadius()))
						{
							currentBall=(JG_Ball *) ballsArray->objectAtIndex(k);
							currentBall->SetInitialTouchPosition(currentBall->getPosition());
						
						}// end of ball collision cheking
					}// end of ball looping
				}// end of hand collision checking
			}// end of hand looping
		}
	}// end of touch looping
}
	

//TODO: implement for multi touch
//TODO: clean up and make it more modular
void JG_Main_Game::ccTouchesMoved(CCSet* pTouches, CCEvent* event)
{
	
	if(currentBall==NULL || currentHand == NULL)
		return;

	// if direction is not yet set for the ball. ( for now we only have one ball)
	if(!bDirIsSet)
	{

		// find direction based on ball's initialTouchPosition and current touch position
		CCPoint direction=(((CCTouch*) pTouches->anyObject())->getLocation())-currentBall->GetInitialTouchPosition();


		// calculate the direction in Radian
		//********************* TODO: find a better soloution for this ****************/
		float directionRad = atan(direction.y/direction.x);
		if(getSign(direction.x)<0 )
			directionRad+= CC_DEGREES_TO_RADIANS(180);

		if(directionRad<0)
			directionRad+= CC_DEGREES_TO_RADIANS(360);
		/*****************************************************************************/


		float directionDeg = CC_RADIANS_TO_DEGREES(directionRad);

		/************************* Checking Direction Validation **************************/
		if(directionDeg>45&& directionDeg<135)
		{
			//direction up
			if (currentBall->GetBallDirection() != EDir_LeftHandToUp && currentBall->GetBallDirection()!=EDir_RightHandToUp)
			{
				if(currentHand==leftHand && currentBall->GetBallDirection()==EDir_RighHandtToLeft)
				{
					
					currentBall->setPosition(currentHand->getPosition());
					currentBall->MoveCurve(1,rightHand->getPosition());
					bDirIsSet = true;
				
				}
				else if (currentHand==rightHand && currentBall->GetBallDirection()==EDir_LeftHandToRight)
				{
					currentBall->setPosition(currentHand->getPosition());
					currentBall->MoveCurve(1,leftHand->getPosition());
					bDirIsSet = true;
				}
			}
		
		}
		else if(directionDeg>135&&directionDeg<225)
		{
			//direction left
			if (currentBall->GetBallDirection() == EDir_LeftHandToUp)
			{
				
				if(currentHand==leftHand)
				{
					//invalid
				}
				else if (currentHand==rightHand)
				{
					currentBall->setPosition(currentHand->getPosition());
					currentBall->MoveStaight(1,leftHand->getPosition());
					bDirIsSet = true;
				}

			}

		}
		else if(directionDeg>315||directionDeg<45)
		{
			//direction right

			if (currentBall->GetBallDirection() == EDir_RightHandToUp)
			{
				if(currentHand==leftHand)
				{
					currentBall->setPosition(currentHand->getPosition());
					currentBall->MoveStaight(1,rightHand->getPosition());
					bDirIsSet = true;
				}
				else if (currentHand==rightHand)
				{
					//invalid
			
				}
			}
		}
		else
		{
			//invalid
			//CCLOG("invalid is here, direction up");
		}
		/************************* /Checking Direction Validation **************************/

	}
}

// for now just reset everything
void JG_Main_Game::ccTouchesEnded(CCSet* pTouches, CCEvent* event)
{
	currentBall = NULL;
	currentHand = NULL;
	bDirIsSet = false;
}

bool JG_Main_Game::ArePointsColliding(CCPoint point1,CCPoint point2,float radius)
{
	return point1.getDistance(point2)<radius;
}


void JG_Main_Game::TestTouch()
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
	ccTouchesEnded(NULL,NULL);
	
	
	

}
void JG_Main_Game::update(float dt)
{
	//TestTouch();
}