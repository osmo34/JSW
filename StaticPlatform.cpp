#include "StaticPlatform.h"

StaticPlatform::~StaticPlatform() {}

void StaticPlatform::setTravelator(bool isTravelator) {
	this->isTravelator = isTravelator;
}

bool StaticPlatform::getTravelator()	{
	return isTravelator;
}
