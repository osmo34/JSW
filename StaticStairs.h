#pragma once
#include "StaticObject.h"
class StaticStairs : public StaticObject
{
private:
	float posX;
	float posY;

public:
	StaticStairs(const int screenWidth,
		const int screenHeight,
		sf::Texture texture,
		const float posX,
		const float posY,
		const char rightAngle, const float, const float);

	bool isRightAngle;

	~StaticStairs();
};

