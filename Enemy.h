#pragma once
#include "Sprite.h"
#include "DataRoom.h"

class Enemy : public Sprite
{
public:
	Enemy(const int screenWidth,
		const int screenHeight,
		sf::Texture texture,
		RoomData &roomData		
		);
	~Enemy();
	virtual void update(const float dt) = 0;
	void setStartPosition();
	bool isHarmful();
	char objectId;

protected:
	float posX;
	float posY;
	float clampXRight; 
	float clampXLeft;
	float clampYTop;  
	float clampYBottom;
	float speedX;
	float speedY;
	sf::Vector2f movementSpeed;
	virtual void reverseDirection() = 0;
	virtual void setUpEnemy();
};

