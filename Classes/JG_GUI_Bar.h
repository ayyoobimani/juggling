#pragma once
#include "sprite_nodes/CCSprite.h"
#include "cocos2d.h"

using namespace cocos2d;
class JG_GUI_Bar :
	public CCSprite
{
	CCPoint initialPosition;
	CCString barTexture;
	float maxBarScale;
public:

	JG_GUI_Bar(void);
	~JG_GUI_Bar(void);


	

	static JG_GUI_Bar* CreateBar(CCPoint intialPosition,float newMaxScale);
	void InitBar(CCPoint initialPosition,float newMaxScale);
	void SetBarScale(float scale);


};

