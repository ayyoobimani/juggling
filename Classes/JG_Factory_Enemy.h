#pragma once

//#include "JG_Enemy_Base.h"
#include "JG_Factory_Base.h"

class JG_Enemy_Base;



template< class EnemyType>
class JG_Factory_Enemy: public JG_Factory_Base
{
public:

	//TODO: find out why I can't implement function in .cpp
	JG_Factory_Enemy(void){}
	virtual ~JG_Factory_Enemy(void){}

	CCNode* Create()
	{
		return  new EnemyType; 
	}
};

