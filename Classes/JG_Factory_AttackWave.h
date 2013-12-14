#pragma once
#include "JG_Factory_Base.h"


template< class T>
class JG_Factory_AttackWave: public JG_Factory_Base
{
public:
	JG_Factory_AttackWave(void){}
	~JG_Factory_AttackWave(void){}
	CCNode* Create()
	{
		return new T;
	}
	
};

