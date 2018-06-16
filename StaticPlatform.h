#pragma once
#include "StaticObject.h"
class StaticPlatform : public StaticObject
{
private:
	float posX;
	float posY;
	bool isTravelator;

public:
	using StaticObject::StaticObject;
	~StaticPlatform();
	const char objectId = 't';
	void setTravelator(bool isTravelator);
	bool getTravelator();
};

