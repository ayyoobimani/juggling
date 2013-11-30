#include "JG_ScorePopup.h"

JG_ScorePopup:: JG_ScorePopup()
{
}

JG_ScorePopup:: ~JG_ScorePopup()
{
}


JG_ScorePopup* JG_ScorePopup::CreateScorePopup(JG_Game_Main* game, int score , int multiplier , CCPoint position)
{

	JG_ScorePopup * scorePopup = new JG_ScorePopup();
	if (scorePopup)
	{
		
		scorePopup->autorelease();
		scorePopup->setPosition(position);
		scorePopup->mainGame = game;
		scorePopup->mainGame->addChild(scorePopup);
		scorePopup->retain();
		scorePopup->scoreLabel =CCLabelBMFont::create ("0", "fonts/font.fnt",  CCDirector::sharedDirector()->getWinSize().height * 0.1f);
		scorePopup->setScale(0.5);
		scorePopup->addChild(scorePopup->scoreLabel);

		scorePopup->ScoreGainAnimation = CCSequence::create(
			CCEaseInOut::create(CCScaleTo::create(0.3,2,2),0.5)
			,CCEaseInOut::create(CCScaleTo::create(0.3,1,1),0.5),NULL);
		scorePopup->ScoreGainAnimation->retain();

		scorePopup->initialScore(score , multiplier);
		scorePopup->scheduleOnce(schedule_selector(JG_ScorePopup::removeFromScreen),SCORE_EXISTENCE_DURATION);

		return scorePopup;

	}
	CC_SAFE_DELETE(scorePopup);
	return NULL;	
}

void JG_ScorePopup::removeFromScreen(float dt)
{
	mainGame->removeChild(this);
	CC_SAFE_RELEASE(this);
}

void JG_ScorePopup::initialScore(int score , int multiplier)
{

	if (multiplier==1)
	{
		scoreLabel->setString(CCString::createWithFormat("%i" , score)->getCString());
	}
	else
	{
		scoreLabel->setString(CCString::createWithFormat("%i:%i" , multiplier , score )->getCString());
		
	}

	scoreLabel->runAction((CCAction *)ScoreGainAnimation);
}