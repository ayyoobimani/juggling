#pragma once

#include "cocos2d.h"
#include "sprite_nodes\CCSprite.h"
#include "JG_Enemy_Base.h"

class JG_Enemy_Base;
using namespace cocos2d;

class JG_Factory_Base
{
public:
	JG_Factory_Base(void);
	virtual ~JG_Factory_Base(void);

	//Warning : this factory only works when objects are derived from CCSprite 
	virtual JG_Enemy_Base* Create()= 0;
};
