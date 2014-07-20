#include "JG_Enemy_Crow.h"


JG_Enemy_Crow::JG_Enemy_Crow(void) 
	: JG_Enemy_Base()
{
	spriteFileName = "Enemies/Triangle/triangle.png";

	attackingAnimationFolder = "Enemies/UFO/Attacking/";
	attackingAnimationName = "UFO_1_Attack";

	dyingAnimationFolder = "Enemies/UFO/Dying/";
	dyingAnimationName = "UFO_1_Die";

	escapingAnimationFolder = "Enemies/UFO/Escaping/";
	escapingAnimationName = "UFO_1_Escape";

	intendingAnimationFolder = "Enemies/UFO/Intending/";
	intendingAnimationName = "UFO_1_Intend";

	landingAnimationFolder = "Enemies/UFO/Landing/";
	landingAnimationName = "UFO_1_Land";

	waitingAnimationFolder = "Enemies/UFO/Waiting/";
	waitingAnimationName = "UFO_1_Wait";

	landingTime = 2;

}


JG_Enemy_Crow::~JG_Enemy_Crow(void)
{
}






