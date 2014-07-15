#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class JG_Button :
	public CCMenuItemSprite
{
public:
	static JG_Button * Create(CCNode* normalSprite, CCNode* selectedSprite, CCObject* target, SEL_MenuHandler selector, CCString soundEffect);

	void SetSoundEffect(CCString sound);

	void selected();

	JG_Button(void);
	~JG_Button(void);
private:
	CCString soundEffect;
};

