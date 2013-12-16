#include "JG_Path.h"


JG_Path::JG_Path(void)
{
	health = 100;
	bIsPathEnabled = true;
	
}


JG_Path::~JG_Path(void)
{
	//CCSprite::~CCSprite();
	unscheduleAllSelectors();

}

JG_Path * JG_Path::CreatePath(JG_Game_Main* game,float power,CCPoint origin , CCPoint destination)
{
	JG_Path * path = new JG_Path();
	if (path)
	{
		path->autorelease();
		path->InitialPath(game,power,origin,destination);
		path->scheduleUpdate();
		


		return path;
	}
	CC_SAFE_DELETE(path);
	return NULL;

}

void JG_Path::InitialPath(JG_Game_Main* game,float power,CCPoint origin , CCPoint destination)
{
	mainGame = game;
	bMustHighlight = false;
	pathThrowPower = power;
	originPoint = origin;
	destinationPoint = destination;
	pathThrowSpeed = JG_Ball::minSpeed + JG_Ball::minSpeed * pathThrowPower;
	pathThrowRadian = JG_Ball::CalculateCurveRad(pathThrowSpeed,originPoint,destinationPoint);

	tracePointTexture = CCTextureCache::sharedTextureCache()->addImage("deadStar.png");
	traceLivePointTexture = CCTextureCache::sharedTextureCache()->addImage("liveStar.png");

	this->schedule(schedule_selector(JG_Path::GiveScoreToPlayer),CalculateScoreInterval());
}

void JG_Path::draw()
{
	if(IsPathEnabled())
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
	PointPosition.x=abs(destinationPoint.x-originPoint.x)*lenghtRatio+originPoint.x;
	PointPosition.y=(PointPosition.x*tan(pathThrowRadian))-(0.5*GRAVITY*pow(PointPosition.x,2)/pow(pathThrowSpeed*cos(pathThrowRadian),2))+destinationPoint.y;

	return PointPosition;

}

void JG_Path::TakeDamage(float damage)
{
	health-= damage;
	//CCLOG("health is %f",health);
	if(health<=0)
	{
		mainGame->OnPathLost(this);
	
	}
}


void JG_Path::SetHighlight(bool newHighlight)
{
	bMustHighlight = newHighlight;

}


void JG_Path::GiveScoreToPlayer(float dt)
{
	unschedule(schedule_selector(JG_Path::GiveScoreToPlayer));
	currentScore = CalculateScore();
	mainGame->ManagePathScore(this);
	schedule(schedule_selector(JG_Path::GiveScoreToPlayer),CalculateScoreInterval());
	
}


int JG_Path::GetScore()
{
	return currentScore;
}


float JG_Path::CalculateScoreInterval()
{
	return SCOREINTERVAL_BASE + (SCOREINTERVAL_RANDOM_OFFSET * CCRANDOM_0_1());
}

int JG_Path::CalculateScore()
{
	return SCORE_BASE;

}

float JG_Path::GetHealth()
{
	return health;
}

void JG_Path::SetPathEnable(bool enable)
{
	bIsPathEnabled = enable;
	if(!bIsPathEnabled)
		DisablePath();

}

bool JG_Path::IsPathEnabled()
{
	return bIsPathEnabled;
}

void JG_Path::DisablePath()
{
	unschedule(schedule_selector(JG_Path::GiveScoreToPlayer));
}

void JG_Path::EnablePath()
{

}