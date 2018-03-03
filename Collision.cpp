#include "Collision.h"
		
Collision::Collision() {
	playerLeft = 0.0f, playerRight = 0.0f, playerTop = 0.0f, playerBottom = 0.0f;
}						  

Collision::~Collision()	{ }

// entities - enemies, pickups.
sf::RectangleShape Collision::updatePositions(std::vector<ObjectPositions> &t, ObjectPositions &m_objectPosition, sf::RectangleShape collisionBox) {
	t.clear();
	t.shrink_to_fit();
	t.push_back(m_objectPosition);
	for (auto &it : t) {
		collisionBox.setSize(sf::Vector2f(std::fabs(it.left - it.right), std::fabs(it.top - it.bottom)));
		collisionBox.setFillColor(sf::Color::Red);
		collisionBox.setPosition(it.left, it.bottom);
	}
	return collisionBox;
}

// player
void Collision::updatePositions(ObjectPositions m_objectPositions) {
	playerLeft = m_objectPositions.left, playerRight = m_objectPositions.right;
	playerTop = m_objectPositions.top, playerBottom = m_objectPositions.bottom;
	playerRectangle.setSize(sf::Vector2f(std::fabs(playerLeft - playerRight), std::fabs(playerTop - playerBottom)));
	playerRectangle.setPosition(playerLeft, playerBottom);	
}

// essentially a fake template, uses an object id to update position (if an update is required). 
void Collision::updateObjectPosition(std::function<double(char c)> position, char t) {
	const char PLAYER = 'p', STATIC_OBJECT = 's', ENEMY_MOVING = 'e', PICK_UP = 'u';
	ObjectPositions m_objectPosition;
	m_objectPosition.top = position(TOP);
	m_objectPosition.bottom = position(BOTTOM);
	m_objectPosition.left = position(LEFT);
	m_objectPosition.right = position(RIGHT);

	switch (t) {
	case PLAYER:
		updatePositions(m_objectPosition);
		break;
	case STATIC_OBJECT:
		staticObjectPositions.push_back(m_objectPosition);
		break;
	case ENEMY_MOVING:
		collisionRectangle = updatePositions(entityPositions, m_objectPosition, collisionRectangle);
		break;
	case PICK_UP:
		collisionRectangle = updatePositions(entityPositions, m_objectPosition, collisionRectangle);
		break;
	default: // This should never happen!
		std::cout << "broke";
		break;
	}
}

// detects collision with an entity
bool Collision::checkCollision() {
	return (playerRectangle.getGlobalBounds().intersects(collisionRectangle.getGlobalBounds())) ? true : false;
}

// check static blocks
void Collision::checkCollision(std::function<void(char c, float i)> playerCollision) {
	
	for (auto it : staticObjectPositions) {
		
		if (playerTop >= it.bottom && playerBottom >= it.top) {
			playerCollision(NO_COLLISION, NO_CHANGE_GROUND_HEIGHT);
			return;
		} else if (playerBottom >= it.top && playerBottom < it.top + COLLISION_OFFSET &&
			playerLeft <= it.right && playerRight >= it.left) {
			playerCollision(TOP, it.top);
			return;
		} else if (playerTop <= it.bottom && playerTop >= it.bottom - COLLISION_OFFSET &&
			playerLeft <= it.right && playerRight >= it.left) {
			playerCollision(BOTTOM, 0.0);
			return;
		}
		if (playerTop >= it.top) {
			if (playerLeft <= it.right && playerRight >= it.right) {
				playerCollision(LEFT, NO_CHANGE_GROUND_HEIGHT);
				return;
			} else if (playerRight >= it.left && playerLeft <= it.left) {
				playerCollision(RIGHT, NO_CHANGE_GROUND_HEIGHT);
				return;
			}
		}
		else {
			playerCollision(NO_COLLISION, NO_CHANGE_GROUND_HEIGHT);
		}
	}	
}

// debugging purposes
void Collision::draw(sf::RenderWindow * window)
{
	window->draw(playerRectangle);
}




