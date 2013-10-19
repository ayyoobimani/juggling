#include "JG_TempLineContainer.h";

using namespace cocos2d;



JG_TempLineContainer::JG_TempLineContainer() {

	_dash = 10;
	_dashSpace = 10;
	_lineType = LINE_NONE;
	_screenSize = CCDirector::sharedDirector()->getWinSize();
	_energyLineX = _screenSize.width * 0.96f;
	_energyHeight = _screenSize.height * 0.8f;

	glLineWidth(8.0 * CC_CONTENT_SCALE_FACTOR());

	this->reset();
}

JG_TempLineContainer* JG_TempLineContainer::create () {
	JG_TempLineContainer * node = new JG_TempLineContainer();
	if (node) {
		node->autorelease();
		return node;
	}
	CC_SAFE_DELETE(node);
	return NULL;
}

void JG_TempLineContainer::reset() {
	_energy = 1.0;
	_energyDecrement = 0.015f;
}

void JG_TempLineContainer::update (float dt) {
	_energy -= dt * _energyDecrement;
	if (_energy < 0) _energy = 0;
}

void JG_TempLineContainer::setEnergyDecrement(float value) {
	_energyDecrement += value;
	if (_energyDecrement > 0.07) _energyDecrement = 0.07;
}

void JG_TempLineContainer::draw () 
{

	ccDrawColor4F(1.0, 1.0, 1.0, 1.0);
	ccDrawLine(_tip, _pivot);
	ccDrawCircle(_pivot, 10, CC_DEGREES_TO_RADIANS(360), 10, false);
}

