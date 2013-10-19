#include "JG_Hand.h"

JG_Hand::JG_Hand()
{
	radius = 32;
}

JG_Hand::~JG_Hand()
{

}

JG_Hand* JG_Hand::createWithFileName(const char * pszFileName,CCPoint initialPos) 
{
    
    JG_Hand * hand = new JG_Hand();
	if (hand && hand->initWithFile(pszFileName))
	{
		hand->autorelease();
		hand->setPosition(initialPos);
		return hand;
	}
	CC_SAFE_DELETE(hand);
	return NULL;
}