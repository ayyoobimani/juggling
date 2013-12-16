#pragma once


#include "cocos2d.h"
#include "JG_Game_Main.h"

using namespace cocos2d;


class JG_Game_Main;

#define SCORE_EXISTENCE_DURATION 1.5

class JG_ScorePopup: public CCNode
{
	private:
	CCLabelBMFont * scoreLabel;
	CCFiniteTimeAction* ScoreGainAnimation;
	JG_Game_Main * mainGame;

	void removeFromScreen(float dt);
	void initialScore(int score , int multiplier);

public:
	JG_ScorePopup();
	~JG_ScorePopup();

	static JG_ScorePopup* CreateScorePopup(JG_Game_Main * game ,int score , int multiplier , CCPoint position );
};
