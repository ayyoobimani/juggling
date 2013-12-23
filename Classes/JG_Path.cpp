#include "JG_Path.h"

std::vector<PathHealthStatesForEachLevel> JG_Path::pathHealthStatesForEachLevel;

JG_Path::JG_Path(void)
{


	
}


JG_Path::~JG_Path(void)
{
	//CCSprite::~CCSprite();
	unscheduleAllSelectors();

}

void JG_Path::InitialPathHealthStatesForEachLevel()
{
	pathHealthStatesForEachLevel.push_back(PathHealthStatesForEachLevel());
	pathHealthStatesForEachLevel.push_back(PathHealthStatesForEachLevel());
	pathHealthStatesForEachLevel.push_back(PathHealthStatesForEachLevel());
	pathHealthStatesForEachLevel.push_back(PathHealthStatesForEachLevel());

	pathHealthStatesForEachLevel[0].healthStateTextures.push_back
		(CCTextureCache::sharedTextureCache()->addImage("Paths/1-1.png"));
	pathHealthStatesForEachLevel[0].healthStateTextures.push_back
		(CCTextureCache::sharedTextureCache()->addImage("Paths/1-2.png"));
	pathHealthStatesForEachLevel[0].healthStateTextures.push_back
		(CCTextureCache::sharedTextureCache()->addImage("Paths/1-3.png"));
	pathHealthStatesForEachLevel[0].healthStateTextures.push_back
		(CCTextureCache::sharedTextureCache()->addImage("Paths/1-4.png"));

	pathHealthStatesForEachLevel[1].healthStateTextures.push_back
		(CCTextureCache::sharedTextureCache()->addImage("Paths/2-1.png"));
	pathHealthStatesForEachLevel[1].healthStateTextures.push_back
		(CCTextureCache::sharedTextureCache()->addImage("Paths/2-2.png"));
	pathHealthStatesForEachLevel[1].healthStateTextures.push_back
		(CCTextureCache::sharedTextureCache()->addImage("Paths/2-3.png"));
	pathHealthStatesForEachLevel[1].healthStateTextures.push_back
		(CCTextureCache::sharedTextureCache()->addImage("Paths/2-4.png"));

	pathHealthStatesForEachLevel[2].healthStateTextures.push_back
		(CCTextureCache::sharedTextureCache()->addImage("Paths/3-1.png"));
	pathHealthStatesForEachLevel[2].healthStateTextures.push_back
		(CCTextureCache::sharedTextureCache()->addImage("Paths/3-2.png"));
	pathHealthStatesForEachLevel[2].healthStateTextures.push_back
		(CCTextureCache::sharedTextureCache()->addImage("Paths/3-3.png"));
	pathHealthStatesForEachLevel[2].healthStateTextures.push_back
		(CCTextureCache::sharedTextureCache()->addImage("Paths/3-4.png"));

	pathHealthStatesForEachLevel[3].healthStateTextures.push_back
		(CCTextureCache::sharedTextureCache()->addImage("Paths/4-1.png"));
	pathHealthStatesForEachLevel[3].healthStateTextures.push_back
		(CCTextureCache::sharedTextureCache()->addImage("Paths/4-2.png"));
	pathHealthStatesForEachLevel[3].healthStateTextures.push_back
		(CCTextureCache::sharedTextureCache()->addImage("Paths/4-3.png"));
	pathHealthStatesForEachLevel[3].healthStateTextures.push_back
		(CCTextureCache::sharedTextureCache()->addImage("Paths/4-4.png"));
}

JG_Path * JG_Path::CreatePath(JG_Game_Main* game,float power,CCPoint origin , CCPoint destination)
{
	JG_Path * path = new JG_Path();
	if (path && path->initWithFile("cross.png"))
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
	pathLevel = mainGame->GetPathLevelByPower(power);
	pathHealth = MAX_HEALTH;
	bMustHighlight = false;
	bIsPathEnabled = true;
	pathThrowPower = power;
	originPoint = origin;
	destinationPoint = destination;
	pathThrowSpeed = JG_Ball::minSpeed + JG_Ball::minSpeed * pathThrowPower;
	pathThrowRadian = JG_Ball::CalculateCurveRad(pathThrowSpeed,originPoint,destinationPoint);

	tracePointTexture = CCTextureCache::sharedTextureCache()->addImage("deadStar.png");
	traceLivePointTexture = CCTextureCache::sharedTextureCache()->addImage("liveStar.png");

	setPosition(originPoint);

	//initWithFile("Paths/4-1.png");
	setAnchorPoint(ccp(0,0));
	UpdatePathHealthStateTexture();
	

	this->schedule(schedule_selector(JG_Path::GiveScoreToPlayer),CalculateScoreInterval());
}

void JG_Path::UpdatePathHealthStateTexture()
{
	if( pathHealthStatesForEachLevel.size()>pathLevel)
	{
		int healthStateTexturesLength = pathHealthStatesForEachLevel[pathLevel].healthStateTextures.size();
		if(healthStateTexturesLength==0)
			return;
	
		int currentHealthStateIndex = healthStateTexturesLength- ((pathHealth/MAX_HEALTH)*healthStateTexturesLength);
		currentHealthStateIndex=clampf(currentHealthStateIndex,0,healthStateTexturesLength-1);
			
		setTexture(pathHealthStatesForEachLevel[pathLevel].healthStateTextures[currentHealthStateIndex]);
	}
}

void JG_Path::draw()
{
	
	if(IsPathEnabled())
		DrawPath();
}

void JG_Path::DrawPath()
{
	getTexture()->drawAtPoint(ccp(0,0));
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
		//else
			//tracePointTexture->drawAtPoint(convertToNodeSpace(tracePoint));
	}

}

CCPoint JG_Path::GetPositionForLengthRatio(float lenghtRatio)
{
	//TODO: implement this
	CCPoint localPosition;
	CCPoint globalPosition;
	lenghtRatio = clampf(lenghtRatio,0,1);

	localPosition.x=abs(destinationPoint.x-originPoint.x)*lenghtRatio;
	localPosition.y=(localPosition.x*tan(pathThrowRadian))-(0.5*GRAVITY*pow(localPosition.x,2)/pow(pathThrowSpeed*cos(pathThrowRadian),2));

	globalPosition=originPoint+localPosition;
	return globalPosition;

}

void JG_Path::TakeDamage(float damage)
{
	pathHealth-= damage;
	SetHealth(pathHealth-damage);
	//CCLOG("health is %f",health);
	if(pathHealth<=0)
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
	return pathHealth;
}

void JG_Path::SetHealth(float newHealth)
{
	pathHealth=newHealth;
	UpdatePathHealthStateTexture();
}

void JG_Path::SetPathEnable(bool enable)
{
	bIsPathEnabled = enable;
	SetScoringEnable(enable);
	

}

bool JG_Path::IsPathEnabled()
{
	return bIsPathEnabled;
}

void JG_Path::SetScoringEnable(bool bEnable)
{
	if(bEnable)
		schedule(schedule_selector(JG_Path::GiveScoreToPlayer),CalculateScoreInterval());
	else
		unschedule(schedule_selector(JG_Path::GiveScoreToPlayer));

}


void JG_Path::ResetPath()
{
	SetPathEnable(true);
	SetHealth(MAX_HEALTH);
}
