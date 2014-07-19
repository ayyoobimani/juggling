#pragma once
#include "SimpleAudioEngine.h"
#include "cocoa/CCString.h"

using namespace cocos2d;
class JG_SoundEngine
{
public:
	JG_SoundEngine(void);
	~JG_SoundEngine(void);

	static void playMusic(CCString backsound);
	static void stopMusic();
	static void resumeMusic();
	static void pauseMusic();
	static void playSoundEffect(CCString effectsound);
};

