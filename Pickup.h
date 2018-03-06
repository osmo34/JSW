#pragma once
#include "Sprite.h"
class Pickup : public Sprite
{
public:
	Pickup(const int screenWidth,
		const int screenHeight,
		sf::Texture texture,
		const float posX,
		const float posY,
		const char objectId, 
		const float, const float);
	~Pickup();

	char objectId;
	void update(const float dt);

private:							
	void setStartPosition();
	float posX;
	float posY;
};

