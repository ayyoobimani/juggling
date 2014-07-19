#pragma once
#include "sprite_nodes/CCSprite.h"
#include "JG_Game_Main.h"


class JG_Game_Main;
using namespace cocos2d;

#define RADIUS_RATIO 0.5

class JG_Hand : public CCSprite//adding a comment
{

	bool bMustDrawArea;

	float throwPower;

	CCSprite *touchLayer;
	
public:
	JG_Hand(void);
	virtual ~JG_Hand(void);

	static JG_Hand* CreateHand(JG_Game_Main* game, CCPoint initialPos, const char * handSprite, const char * touchSprite);

	JG_Game_Main* mainGame;

	float radius;
	
	float GetRadius()
	{
		return radius;
	}


	

	 CREATE_FUNC(JG_Hand);

	 void draw();

	 void SetAreaVisibility(bool newVisibility);

	 /*sets the last power this hand has thrown a ball upward */
	 void setThrowPower(float _power);
	 /*resets the last power this hand has thrown a ball upward */
	 void resetThrowPower (float dt);
	 /*return the last power this hand has thrown a ball upward*/
	 float getThrowPower();
	 void initTouchLayer(const char * touchSprite);
	 void SetTouchLayerVisible(bool mustShow);
	 
};


