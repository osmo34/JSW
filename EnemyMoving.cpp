#include "EnemyMoving.h"

EnemyMoving::~EnemyMoving()	{}

void EnemyMoving::setUpEnemy() {
	movementSpeed.x = speedX;
	movementSpeed.y = speedY;
}

void EnemyMoving::update(const float dt) {
	m_sprite.move(movementSpeed * dt);
	reverseDirection();
}

void EnemyMoving::reverseDirection() {
	// TODO: this is for testing only, this would be dependent on the length of the platform
	if (m_sprite.getPosition().x >= m_screenWidth) {
		movementSpeed.x = -speedX;		
	}
	else if (m_sprite.getPosition().x <= 0) {
		movementSpeed.x = speedX;
	}

	if (m_sprite.getPosition().y >= m_screenHeight) {
		movementSpeed.y = -speedY;
	}
	else if (m_sprite.getPosition().y <= 0) {
		movementSpeed.y = speedY;
	}
}

