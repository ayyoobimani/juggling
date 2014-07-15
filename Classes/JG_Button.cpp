#include "JG_Button.h"
#include "SimpleAudioEngine.h"


JG_Button * JG_Button::Create(CCNode* normalSprite, CCNode* selectedSprite, CCObject* target, SEL_MenuHandler selector, CCString soundEffect)
{
	JG_Button * button = new JG_Button();

	button->initWithNormalSprite(normalSprite, selectedSprite, nullptr, target, selector); 
	button->SetSoundEffect(soundEffect);

	button->autorelease();
	return button;
}

JG_Button::JG_Button(void)
{
}


JG_Button::~JG_Button(void)
{
}

void JG_Button::SetSoundEffect(CCString sound)
{
	soundEffect = sound;
}

void JG_Button::selected()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(soundEffect.getCString());
	CCMenuItemSprite::selected();
}
