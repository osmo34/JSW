#include "Enemy.h"

Enemy::Enemy(int screenWidth, int screenHeight,	sf::Texture texture,
	float posX,  float posY, float speedX, float speedY) : 
	posX(posX), posY(posY), 
	speedX(speedX), speedY(speedY), 
	Sprite(screenWidth, screenHeight, texture) {
	setStartPosition();
	setUpEnemy();
}

Enemy::~Enemy(){ }

void Enemy::setStartPosition() {
	m_sprite.setPosition(sf::Vector2f(posX, posY));
	m_sprite.setOrigin(sf::Vector2f(0, 0));
}

// default behaviour
void Enemy::setUpEnemy() {
	movementSpeed.x = speedX;
	movementSpeed.y = speedY;
}
