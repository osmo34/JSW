#pragma once
#include "Sprite.h"
class Enemy : public Sprite
{
public:
	Enemy(const int screenWidth,
		const int screenHeight,
		sf::Texture texture,
		const float posX,
		const float posY,
		const char objectId,
		const float speedX,
		const float speedY
		);
	~Enemy();
	virtual void update(const float dt) = 0;
	void setStartPosition();
	bool isHarmful();
	char objectId;

protected:
	float posX;
	float posY;
	float speedX;
	float speedY;
	sf::Vector2f movementSpeed;
	virtual void reverseDirection() = 0;
	virtual void setUpEnemy();
};

