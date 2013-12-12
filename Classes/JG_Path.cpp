#include "JG_Path.h"


JG_Path::JG_Path(void)
{
}


JG_Path::~JG_Path(void)
{
}

JG_Path * JG_Path::CreatePath(float power,CCPoint origin , CCPoint destination)
{
	JG_Path * path = new JG_Path();
	if (path)
	{
		path->autorelease();
		path->InitialPath(power,origin,destination);
		path->scheduleUpdate();


		return path;
	}
	CC_SAFE_DELETE(path);
	return NULL;

}

void JG_Path::InitialPath(float power,CCPoint origin , CCPoint destination)
{
	bMustHighlight = false;
	pathThrowPower = power;
	originPoint = origin;
	destinationPoint = destination;
	pathThrowSpeed = JG_Ball::minSpeed + JG_Ball::minSpeed * pathThrowPower;
	pathThrowRadian = JG_Ball::CalculateCurveRad(pathThrowSpeed,originPoint,destinationPoint);

	tracePointTexture = CCTextureCache::sharedTextureCache()->addImage("deadStar.png");
	traceLivePointTexture = CCTextureCache::sharedTextureCache()->addImage("liveStar.png");
}

void JG_Path::draw()
{
	DrawPath();
}

void JG_Path::DrawPath()
{
	//CCLOG(CCString::createWithFormat("chosen:%f",_power)->getCString());

	float tempSpeedX,tempSpeedY;
	CCPoint tracePoint;


	tracePoint = originPoint;
	tempSpeedX= pathThrowSpeed * cos(pathThrowRadian);
	tempSpeedY = pathThrowSpeed * sin(pathThrowRadian);
	float tempInterval = 0.07;
	//for( int i = 0 ; i< 50 ; i++)
	while(tracePoint.y >= destinationPoint.y)
	{
		tempSpeedY = -GRAVITY* tempInterval  + tempSpeedY;
		//tempSpeedX = tempSpeedX;
		tracePoint.x = tempSpeedX * tempInterval + tracePoint.x;
		tracePoint.y = tempSpeedY * tempInterval + tracePoint.y;

		if(bMustHighlight)
			traceLivePointTexture->drawAtPoint(convertToNodeSpace(tracePoint));
		else
			tracePointTexture->drawAtPoint(convertToNodeSpace(tracePoint));
	}

}

CCPoint JG_Path::GetPositionForLengthRatio(float lenghtRatio)
{
	//TODO: implement this
	CCPoint PointPosition;
	lenghtRatio = clampf(lenghtRatio,0,1);
	PointPosition.x=abs(destinationPoint.x-originPoint.x)*lenghtRatio;
	PointPosition.y=(PointPosition.x*atan(pathThrowRadian))-(0.5*GRAVITY*pow(PointPosition.x,2)/pow(pathThrowSpeed*cos(pathThrowRadian),2))+destinationPoint.y;

	return PointPosition;

}


void JG_Path::SetHighlight(bool newHighlight)
{
	bMustHighlight = newHighlight;
}
