#include "JG_Enemy_Crow.h"


JG_Enemy_Crow::JG_Enemy_Crow(void) 
	: JG_Enemy_Base()
{
	spriteFileName = "Enemies/UFO/UFO.png";
	attackingAnimationFolder = "Enemies/UFO/Attacking/";
	dyingAnimationFolder = "Enemies/UFO/Dying/";
	escapingAnimationFolder = "Enemies/UFO/Escaping/";
	intendingAnimationFolder = "Enemies/UFO/Intending/";
	landingAnimationFolder = "Enemies/UFO/Landing/";
	waitingAnimationFolder = "Enemies/UFO/Waiting/";
	landingTime = 2;
}


JG_Enemy_Crow::~JG_Enemy_Crow(void)
{
}






