#pragma once
#include "sprite_nodes\CCSprite.h"
#include "cocos2d.h"
#include "JG_Game_Main.h"

class JG_Game_Main;

#define SCORE_MAX_OFFSET
#define SCORE_MIN_OFFSET
#define SCORE_BASE 20


#define SCOREINTERVAL_MAX_OFFSET
#define SCOREINTERVAL_MIN_OFFSET
#define SCOREINTERVAL_BASE 2.0




using namespace cocos2d;
class JG_Path :
	public CCSprite
{

	JG_Game_Main* mainGame;
	/*! the power that ball will be thrown in this path */
	float pathThrowPower;

	bool bIsPathEnabled;

	float pathThrowRadian;

	float pathThrowSpeed;

	CCPoint originPoint;
	CCPoint destinationPoint;

	float health; 

	int currentScore;

		
	void GiveScoreToPlayer(float dt);
	float CalculateScoreInterval();
	int CalculateScore();


	// trace texture for drawing throw paths
	CCTexture2D* tracePointTexture;
	CCTexture2D* traceLivePointTexture;

	bool bMustHighlight;

	void DisablePath();
	void EnablePath();

	void DrawPath();
public:
	JG_Path(void);
	virtual ~JG_Path(void);

	static JG_Path* CreatePath(JG_Game_Main* game,float power,CCPoint origin , CCPoint destination);

	void draw();

	void InitialPath(JG_Game_Main* game,float power,CCPoint origin , CCPoint destination);
	
	void SetHighlight(bool newHighlight);

	/*! returns the position for a ratio of path's length ( i.e : the position of 0.5 length of Path) */
	CCPoint GetPositionForLengthRatio(float lenghtRatio);


	void TakeDamage(float damage);
	float GetHealth();

	void SetPathEnable(bool enable);
	bool IsPathEnabled();

	

	int GetScore();

};

