#include "JG_Factory_Enemy.h"

template< class EnemyType>
JG_Factory_Enemy::JG_Factory_Enemy(void)
{
}

template< class EnemyType>
JG_Factory_Enemy::~JG_Factory_Enemy(void)
{
}

template< class EnemyType>
EnemyType * JG_Factory_Enemy::Create()
{
	return (JG_Enemy_Base*) new EnemyType; 
}
