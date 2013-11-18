#include "JG_GUI_Bar.h"

JG_GUI_Bar::JG_GUI_Bar(void)
{
	barTexture="bar.png";
}


JG_GUI_Bar::~JG_GUI_Bar(void)
{
}
JG_GUI_Bar* JG_GUI_Bar::CreateBar(CCPoint initialPosition,float newMaxScale)
{
	JG_GUI_Bar * bar = new JG_GUI_Bar();
	if (bar && bar->init())
	{
		bar->autorelease();
		bar->InitBar(initialPosition,newMaxScale);
		
		return bar;
	}

		CC_SAFE_DELETE(bar);
		return NULL;
}


void JG_GUI_Bar::InitBar(CCPoint initialPosition,float newMaxScale)
{
	

	this->setPosition(initialPosition);
	this->setAnchorPoint(ccp(0.5,0));

	
	
	
	


}
void JG_GUI_Bar::SetBarScale(float scale)
{
	clampf(scale,0,maxBarScale);
	setScaleY(scale);

}


