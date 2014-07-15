#include "JG_SoundEngine.h"


JG_SoundEngine::JG_SoundEngine(void)
{
}
void JG_SoundEngine::playMusic(CCString backsound)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(backsound.getCString(),true);
}

void JG_SoundEngine::stopMusic()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void JG_SoundEngine::resumeMusic()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void JG_SoundEngine::pauseMusic()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void JG_SoundEngine::playSoundEffect(CCString effectsound)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(effectsound.getCString());
}

JG_SoundEngine::~JG_SoundEngine(void)
{
}
