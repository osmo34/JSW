#pragma once
#include "StaticObject.h"
class StaticPlatform : public StaticObject
{
private:
	float posX;
	float posY;

public:
	using StaticObject::StaticObject;
	~StaticPlatform();
	const char objectId = 't';
};

