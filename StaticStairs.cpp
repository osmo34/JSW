#include "StaticStairs.h"

StaticStairs::StaticStairs(const int screenWidth, 
	const int screenHeight, 
	sf::Texture texture, 
	const float posX, 
	const float posY, 	
	const char isRightAngle, const float, const float) : 
	StaticObject(screenWidth, screenHeight, texture, posX, posY, NULL, 0.0f, 0.0f) {
		switch (isRightAngle) {
		case 'l':
			this->isRightAngle = false;
			break;
		case 'r':
			this->isRightAngle = true;
			break;
		default: 
			this->isRightAngle = false;
			break;
		}
	setStartPosition();
}

StaticStairs::~StaticStairs(){}
