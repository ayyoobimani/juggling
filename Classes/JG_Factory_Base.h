#pragma once

#include "cocos2d.h"
#include "sprite_nodes\CCSprite.h"


class JG_Enemy_Base;
using namespace cocos2d;

class JG_Factory_Base
{
public:
	JG_Factory_Base(void);
	virtual ~JG_Factory_Base(void);

	//Warning : this factory only works when objects are derived from CCSprite 
	virtual CCNode* Create()= 0;
};

