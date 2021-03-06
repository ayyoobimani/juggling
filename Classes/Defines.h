#ifndef  _DEFINES_
#define  _DEFINES_

#include "cocos2d.h"
#define GRAVITY CCDirector::sharedDirector()->getWinSize().height * 0.6
using namespace cocos2d;


static float JG_getSign(float input)
{
	if(input > 0.0)
		1;
	return -1;
}

static float JG_abs(float input)
{
	if(input > 0.0)
		return input;

	return 0.0 - input;

}

static bool ArePointsColliding(CCPoint point1,CCPoint point2,float distance)
{
	return point1.getDistance(point2)<distance;
}

static void ScaleSpriteToDesignSize(CCSprite* sprite)
{
	CCSize designSize  = CCEGLView::sharedOpenGLView()->getDesignResolutionSize();
	sprite->setScaleX(designSize.width/sprite->getContentSize().width);
	sprite->setScaleY(designSize.height/sprite->getContentSize().height);
}

#endif 