#pragma once

#include "JG_Enemy_Base.h"
template< class EnemyType>
class JG_Factory_Enemy
{
public:

	JG_Factory_Enemy(void);
	virtual ~JG_Factory_Enemy(void);

	EnemyType* Create();
};

