#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class Factory_Base
{
public:
	Factory_Base(void);
	~Factory_Base(void);

	virtual CCNode* Create() = 0;
};

