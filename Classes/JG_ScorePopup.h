#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class JG_Game_Main;

#define SCORE_EXISTENCE_DURATION 2

class JG_ScorePopup: public CCNode
{
	private:
	CCLabelBMFont * scoreLabel;
	CCFiniteTimeAction* ScoreGainAnimation;

	void removeFromScreen(float dt);
	void initialScore(int score , int multiplier);

public:
	JG_ScorePopup();
	~JG_ScorePopup();

	static JG_ScorePopup* CreateScorePopup(int score , int multiplier , CCPoint position);



};
