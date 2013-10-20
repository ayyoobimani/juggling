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

	CCSprite * backGround = CCSprite::create("HelloWorld.png");


	
	backGround->setPosition(ccp(screenSize.x/2,screenSize.y/2));
	this->addChild(backGround);

	JG_Ball::CalculateSpeedBoundriesBaseOnLength(10);

	// initing  one ball for test
	ballsArray=CCArray::create(JG_Ball::createWithFileName("ball.png",ccp(40,100)),NULL);
	ballsArray->retain();

	for( int i = 0 ; i<ballsArray->count();i++)
	{
		this->addChild((CCNode*)ballsArray->objectAtIndex(i),6);
	}
	
	//TODO: give them sprite
	rightHand = JG_Hand::createWithFileName("hand.png",ccp(screenSize.x-40,40));
	//rightHand->setAnchorPoint(ccp(rightHand->getTextureRect().getMidX(),rightHand->getTextureRect().getMidY()));
	leftHand = JG_Hand::createWithFileName("hand.png",ccp(40,40));
	//leftHand->setAnchorPoint(ccp(leftHand->getTextureRect().getMidX(),leftHand->getTextureRect().getMidY()));
	CCLog(" left hand position is %f", leftHand->getPosition().x);
	CCLog(" right hand position is %f", rightHand->getPosition().x);

	handsArray= CCArray::create(rightHand,leftHand,NULL);
	handsArray->retain();
	for( int i = 0 ; i<handsArray->count();i++)
	{
		this->addChild((CCNode*)handsArray->objectAtIndex(i),2);
	}

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


void JG_Main_Game::ccTouchesBegan(CCSet* pTouches, CCEvent* event)
{
	//TODO: implement this for multi touch
	CCSetIterator i;
	CCTouch* touch;
	CCPoint tap;

	CCLog(" right hand position is %f", rightHand->getPosition().x);
	for( i = pTouches->begin(); i != pTouches->end(); i++) 
	{
		touch = (CCTouch*) (*i);
		if(touch) 
		{
			CCLOG(" touch pos is %f " , touch->getLocation().y);
			//******************* delete me later *******************/
			//currentBall = (JG_Ball*) ballsArray->randomObject();
			//currentHand = rightHand;
			//currentBall->SetInitialTouchPosition(touch->getLocation());
			//return;
			/*******************************************************/
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
						
void JG_Main_Game::ccTouchesMoved(CCSet* pTouches, CCEvent* event)
{
	
	if(currentBall==NULL || currentHand == NULL)
		return;

	if(!bDirIsSet)
	{
		CCPoint directionBorder=(((CCTouch*) pTouches->anyObject())->getLocation())-currentBall->GetInitialTouchPosition();

		//********************* TODO: find a better soloution for this ****************/
		float directionRad = atan(directionBorder.y/directionBorder.x);
		//CCLOG("atan is %f" , CC_RADIANS_TO_DEGREES(directionRad));
		if(getSign(directionBorder.x)<0 )
			directionRad+= CC_DEGREES_TO_RADIANS(180);

		if(directionRad<0)
			directionRad+= CC_DEGREES_TO_RADIANS(360);
		/*****************************************************************************/
		CCLOG("touch dir is %f" , CC_RADIANS_TO_DEGREES(directionRad)); 
		float directionDeg = CC_RADIANS_TO_DEGREES(directionRad);
		//direction
		if(directionDeg>45&& directionDeg<135)
		{
			//direction up
			if(currentHand==leftHand)
			{
				currentBall->MoveCurve(1,rightHand->getPosition());
				bDirIsSet = true;
				
			}
			else if (currentHand==rightHand)
			{
				currentBall->MoveCurve(1,leftHand->getPosition());
				bDirIsSet = true;
			}
		
		}
		else if(directionDeg>135&&directionDeg<225)
		{
			//direction left
			if(currentHand==leftHand)
			{
				//invalid
			}
			else if (currentHand==rightHand)
			{
				currentBall->MoveStaight(1,leftHand->getPosition());
				bDirIsSet = true;
			}
		}
		else if(directionDeg>315||directionDeg<45)
		{
			//direction right
			if(currentHand==leftHand)
			{
				currentBall->MoveStaight(1,rightHand->getPosition());
				bDirIsSet = true;
			}
			else if (currentHand==rightHand)
			{
				//invalid
			
			}
		}
		else
		{
			//invalid
			CCLOG("invalid is here, direction up");
		}
	}
}


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