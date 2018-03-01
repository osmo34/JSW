#include "EnemyHorizontal.h"

EnemyHorizontal::~EnemyHorizontal()	{}

void EnemyHorizontal::setUpEnemy() {
	movementSpeed.x = speedX;
	movementSpeed.y = speedY;
}

void EnemyHorizontal::update(const float dt) {
	m_sprite.move(movementSpeed * dt);
	reverseDirection();
}

void EnemyHorizontal::reverseDirection() {
	// TODO: this is for testing only, this would be dependent on the length of the platform
	if (m_sprite.getPosition().x >= m_screenWidth) {
		movementSpeed.x = -speedX;		
	}
	else if (m_sprite.getPosition().x <= 0) {
		movementSpeed.x = speedX;
	}
}

