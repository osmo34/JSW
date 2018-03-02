#include "Collision.h"
						   
Collision::Collision() {
	playerLeft = 0.0f;
	playerRight = 0.0f;
	playerTop = 0.0f;
	playerBottom = 0.0f;
}						  

Collision::~Collision()	{ }

// essentially a fake template, uses an object id to update position (if an update is required). 
void Collision::updateObjectPosition(std::function<double(char c)> position, char t) {
	const char PLAYER = 'p', STATIC_OBJECT = 's', ENEMY_MOVING = 'e';
	ObjectPositions m_objectPosition;
	m_objectPosition.top = position(TOP);
	m_objectPosition.bottom = position(BOTTOM);
	m_objectPosition.left = position(LEFT);
	m_objectPosition.right = position(RIGHT);

	switch (t) {
	case PLAYER:
		playerLeft = m_objectPosition.left, playerRight = m_objectPosition.right;
		playerTop = m_objectPosition.top, playerBottom = m_objectPosition.bottom;
		break;
	case STATIC_OBJECT:
		staticObjectPositions.push_back(m_objectPosition);
		break;
	case ENEMY_MOVING:
		updatePositions(movingEnemyPositions, m_objectPosition);
		break;
	default: // This should never happen!
		std::cout << "broke";
		break;
	}
}


void Collision::checkCollision(std::function<void(char c, float i)> playerCollision) {	

	for (auto it : staticObjectPositions) {	

		if (playerTop >= it.bottom && playerBottom >= it.top) {
			playerCollision(NO_COLLISION, NO_CHANGE_GROUND_HEIGHT);
			return;
		}
		else if (playerBottom >= it.top && playerBottom < it.top + COLLISION_OFFSET && 
			playerLeft <= it.right && playerRight >= it.left) {
			playerCollision(TOP, it.top);
			return;
		}							   
		else if (playerTop <= it.bottom && playerTop >= it.bottom - COLLISION_OFFSET &&
			playerLeft <= it.right && playerRight >= it.left) {
				playerCollision(BOTTOM, 0.0);
		}							   
		if (playerTop >= it.top) {	
			if (playerLeft <= it.right && playerRight >= it.right)  {
				playerCollision(LEFT, NO_CHANGE_GROUND_HEIGHT);
				return;
			}
			else if (playerRight >= it.left && playerLeft <= it.left) {
				playerCollision(RIGHT, NO_CHANGE_GROUND_HEIGHT);
				return;
			}
		}
		else {
			playerCollision(NO_COLLISION, NO_CHANGE_GROUND_HEIGHT);
		}
	}
}




