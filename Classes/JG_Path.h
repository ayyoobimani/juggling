#pragma once
#include "sprite_nodes\CCSprite.h"
#include "cocos2d.h"
#include "JG_Game_Main.h"

using namespace cocos2d;
class JG_Path :
	public CCSprite
{
	/*! the power that ball will be thrown in this path */
	float pathThrowPower;

	float pathThrowRadian;

	float pathThrowSpeed;

	CCPoint originPoint;
	CCPoint destinationPoint;


	// trace texture for drawing throw paths
	CCTexture2D* tracePointTexture;
	CCTexture2D* traceLivePointTexture;

	bool bMustHighlight;
public:
	JG_Path(void);
	~JG_Path(void);

	static JG_Path* CreatePath(float power,CCPoint origin , CCPoint destination);

	void InitialPath(float power,CCPoint origin , CCPoint destination);
	
	void SetHighlight(bool newHighlight);

	/*! returns the position for a ratio of path's length ( i.e : the position of 0.5 length of Path) */
	CCPoint GetPositionForLengthRatio(float lenghtRatio);

	void draw();
	void DrawPath();
};

