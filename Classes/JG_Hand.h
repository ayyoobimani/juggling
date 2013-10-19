#pragma once
#include "sprite_nodes\CCSprite.h"

using namespace cocos2d;

class JG_Hand : public CCSprite
{
public:
	JG_Hand(void);
	virtual ~JG_Hand(void);

	float radius;
	
	float GetRadius()
	{
		return radius;
	}

	//TODO: change the name
	static JG_Hand* createWithFileName(const char * pszFileName,CCPoint initialPos);

	 CREATE_FUNC(JG_Hand);
};


