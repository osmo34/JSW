#pragma once
#include "Sprite.h"
#include "DataRoom.h"

class Pickup : public Sprite
{
public:
	Pickup(const int screenWidth,
		const int screenHeight,
		sf::Texture texture,
		RoomData &roomData);
	~Pickup();

	char objectId;
	void update(const float dt);

protected:							
	void setStartPosition();
	float posX;
	float posY;
};

