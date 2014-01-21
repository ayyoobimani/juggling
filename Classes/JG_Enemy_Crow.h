#pragma once
#include "JG_Enemy_Base.h"

class JG_Enemy_Crow :
	public JG_Enemy_Base
{
public:
	JG_Enemy_Crow(void);
	~JG_Enemy_Crow(void);

	void InitialIntendingAnimation();
	void InitialLandingAnimation();
	void InitialWaitingAnimation();
	void InitialAttackingAnimation();
	void InitialDyingAnimation();
	void InitialEscapingAnimation();
};

