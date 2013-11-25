#pragma once
#include "sprite_nodes\CCSprite.h"
#include "JG_Game_Main.h"


class JG_Game_Main;
using namespace cocos2d;

#define RADIUS_RATIO 0.5

#define HAND_SCALE 1.0

class JG_Hand : public CCSprite
{

	bool bMustDrawArea;
public:
	JG_Hand(void);
	virtual ~JG_Hand(void);

	JG_Game_Main* mainGame;

	float radius;
	
	float GetRadius()
	{
		return radius;
	}


	static JG_Hand* CreateHand(JG_Game_Main* game, CCPoint initialPos, const char * handSprite);

	 CREATE_FUNC(JG_Hand);

	 void draw();

	 void setDrawAreaFlag(bool);
	 
};


