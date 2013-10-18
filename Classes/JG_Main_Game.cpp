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

	// initing  one ball for test
	CCString test = CCFileUtils::sharedFileUtils()->getSearchPaths().at(0);
	ballArrays=CCArray::create(JG_Ball::createWithFileName("ball.png",ccp(screenSize.x-100,30)),NULL);

	for( int i = 0 ; i<ballArrays->count();i++)
	{
		this->addChild((CCNode*)ballArrays->objectAtIndex(i));
	}
	
	//TODO: give them sprite
	rightHand = JG_Hand::create();
	leftHand = JG_Hand::create();
	handsArrays= CCArray::create(rightHand,leftHand,NULL);
	



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

	for( i = pTouches->begin(); i != pTouches->end(); i++) 
	{
		touch = (CCTouch*) (*i);
		if(touch) 
		{
			JG_Hand * tempHand;
			tap = touch->getLocation();
			for(int j = 0 ; j< handsArrays->count();j++)
			{
				tempHand = (JG_Hand*)handsArrays->objectAtIndex(j);
				//Checking if tap is colliding with any of hands
				if(ArePointsColliding(tap,tempHand->getPosition(),tempHand->GetRadius()))
				{
					currentHand=((JG_Hand*)handsArrays->objectAtIndex(j));
					JG_Ball *testBall;
					//cheking if hand is colliding with a ball
					for (int k=0 ; k<ballArrays->count() ; k++)
					{
						testBall=(JG_Ball *) ballArrays->objectAtIndex(k);
						if(ArePointsColliding(testBall->getPosition(),currentHand->getPosition(),currentHand->GetRadius()))
						{
							currentBall=(JG_Ball *) ballArrays->objectAtIndex(k);
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


}


bool JG_Main_Game::ArePointsColliding(CCPoint point1,CCPoint point2,float radius)
{
	return point1.getDistance(point2)<radius;
}