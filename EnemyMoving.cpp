#include "EnemyMoving.h"

EnemyMoving::~EnemyMoving()	{}

void EnemyMoving::setUpEnemy() {
	movementSpeed.x = speedX;
	movementSpeed.y = speedY;
}

void EnemyMoving::update(const float dt) {
	m_sprite.move(movementSpeed * dt);
	reverseDirection();
	animation->update(dt);
	animation->updateAnimation(dt, m_sourceRect, &m_sprite, isflipLeft);
}

void EnemyMoving::reverseDirection() {
	// TODO: this is for testing only, this would be dependent on the length of the platform
	
	if (m_sprite.getPosition().x >= m_screenWidth) {
		movementSpeed.x = -speedX;
		flipSprite(true);
	}
	else if (m_sprite.getPosition().x <= 0) {
		movementSpeed.x = speedX;
		flipSprite(false);
	}

	if (m_sprite.getPosition().y >= m_screenHeight) {
		movementSpeed.y = -speedY;
	}
	else if (m_sprite.getPosition().y <= 0) {
		movementSpeed.y = speedY;
	}
}

