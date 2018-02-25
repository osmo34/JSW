#pragma once
#include "Sprite.h"
class StaticObject : public Sprite
{
private:
	float posX;
	float posY;

public:
	StaticObject(const int screenWidth, 
		const int screenHeight, 
		sf::Texture texture, 
		const float posX, 
		const float posY);
	~StaticObject();

	void update(const float dt);
	void setStartPosition();
};

