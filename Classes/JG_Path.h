#pragma once
#include "sprite_nodes\CCSprite.h"
#include "cocos2d.h"
#include "JG_Game_Main.h"
#include <vector>

class JG_Game_Main;

#define SCORE_RANDOM_OFFSET 5
#define SCORE_BASE 20



#define SCOREINTERVAL_RANDOM_OFFSET 0.5
#define SCOREINTERVAL_BASE 4.0

#define MAX_HEALTH 100 


struct PathHealthStatesForEachLevel
{
	CCTexture2D* fullTexture;
	CCTexture2D* destroyedTexture;
};


using namespace cocos2d;
class JG_Path :
	public CCSprite
{

	std::vector<CCString> healthStateTextures;

	JG_Game_Main* mainGame;
	/*! the power that ball will be thrown in this path */
	float pathThrowPower;

	int pathLevel;

	bool bIsPathEnabled;

	float pathThrowRadian;

	float pathThrowSpeed;

	CCPoint originPoint;
	CCPoint destinationPoint;

	float pathHealth; 

	int currentScore;

	
	// trace texture for drawing throw paths
	CCTexture2D* tracePointTexture;
	CCTexture2D* traceLivePointTexture;

	bool bMustHighlight;

	void InitPath_HealthStateTextures();
	
		
	void GiveScoreToPlayer(float dt);
	float CalculateScoreInterval();
	int CalculateScore();


	
	void SetScoringEnable(bool bEnable);
	void UpdatePathTextureStatus();
	

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

	static std::vector<PathHealthStatesForEachLevel> pathHealthStatesForEachLevel;
	static void InitialPathHealthStatesForEachLevel();
	//to reset path
	void ResetPath();
	void SetHealth(float newHealth);


};


