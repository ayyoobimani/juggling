#pragma once
#include "sprite_nodes\CCSprite.h"
#include "JG_Game_Main.h"


class JG_Game_Main;
using namespace cocos2d;

class JG_Hand : public CCSprite
{
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
};


