#include "Enemy.h"

Enemy::Enemy(int screenWidth, int screenHeight, sf::Texture texture,
	RoomData &roomData) :
	posX(roomData.positionX), posY(roomData.positionY),
	speedX(roomData.speedX), speedY(roomData.speedY), objectId(roomData.objectType),
	clampXRight(roomData.clampXRight), clampXLeft(roomData.clampXLeft), 
	clampYTop(roomData.clampYTop), clampYBottom(roomData.clampYBottom),
	Sprite(screenWidth, screenHeight, texture) {	
	setStartPosition();
	setUpEnemy();
}

Enemy::~Enemy(){ }

void Enemy::setStartPosition() {
	m_sprite.setPosition(sf::Vector2f(posX, posY));
	m_sprite.setOrigin(sf::Vector2f(0, 0));
}

bool Enemy::isHarmful() {
	return true;
}

// default behaviour
void Enemy::setUpEnemy() {
	movementSpeed.x = speedX;
	movementSpeed.y = speedY;
}
