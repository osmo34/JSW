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

// Update position (can be live or set prior to main loop)
void Collision::updateObjectPosition(std::function<double(char c)> position, char t) {
	const char PLAYER = 'p', STATIC_OBJECT = 's', STATIC_PLATFORM = 't', ENEMY = 'e', ENEMY_MOVING = 'm', ENEMY_STATIC = 'n', PICK_UP = 'u';
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
	case STATIC_PLATFORM:
		staticPlatformPositions.push_back(m_objectPosition);
		break;
	case ENEMY:	
	case ENEMY_MOVING:
	case ENEMY_STATIC:
		collisionRectangle = updatePositions(entityPositions, m_objectPosition, collisionRectangle);
		break;
	case PICK_UP:
		collisionRectangle = updatePositions(entityPositions, m_objectPosition, collisionRectangle);
		break;
	default: // This should never happen!
		std::cout << "error in update object position, check object id" << std::endl;
		break;
	}
}
void Collision::updateObjectPosition(std::function<double(char c)> position, char t, float speed) {
	const char TRAVELATOR = '�';
	ObjectPositions m_objectPosition;
	m_objectPosition.top = position(TOP);
	m_objectPosition.bottom = position(BOTTOM);
	m_objectPosition.left = position(LEFT);
	m_objectPosition.right = position(RIGHT);
	m_objectPosition.speed = speed;

	switch (t) {
	case TRAVELATOR:
		travelatorPositions.push_back(m_objectPosition);
		break;
	default: // This should never happen!
		std::cout << "error in update object position, check object id" << std::endl;
		break;
	}
}

// update position for stairs, overridden as we need to know which direction they are facing
void Collision::updateObjectPosition(std::function<double(char c)> position, bool isRightAngle) {
	ObjectPositions m_objectPosition;
	m_objectPosition.left = position(LEFT);
	m_objectPosition.right = position(RIGHT);
	m_objectPosition.top = position(TOP);
	maxStairHeight = m_objectPosition.top;
	(isRightAngle) ? m_objectPosition.isAngleRight = true : m_objectPosition.isAngleRight = false;
	stairs.push_back(m_objectPosition);
}

// detects collision with an entity
bool Collision::checkCollision() {
	return (playerRectangle.getGlobalBounds().intersects(collisionRectangle.getGlobalBounds())) ? true : false;
}	

void Collision::checkCollisionStairs(sf::Vector2f bottom, sf::Vector2f top, std::function<void(sf::Vector2f b, sf::Vector2f t, bool onStairsBottom, bool onStairsTop, bool isStairsLeft)> playerCheckStairs, bool isLeft)
{
	if (playerBottom < top.y - 64) {
		return;
	}

	if (isLeft) {
		float distanceTop = std::fabs(std::sqrt(std::pow(top.x - playerLeft, 2) + std::pow(top.x - playerRight, 2)));		 
		float distanceBottom = std::fabs(std::sqrt(std::pow(bottom.x - playerLeft, 2) + std::pow(bottom.x - playerRight, 2)));		
		//std::cout << distanceBottom << std::endl;
		if (COLLISION_STAIR_LEFT) { playerCheckStairs(bottom, top, true, false, isLeft); }
		else if (COLLISION_STAIR_LEFT_TOP) { playerCheckStairs(bottom, top, false, true, isLeft); }
		else if (COLLISION_STAIR_LEFT_TOP_EXIT) { playerCheckStairs(bottom, top, true, false, isLeft); }
	}
	else {
		float distanceTop = std::fabs(std::sqrt(std::pow(top.x - playerRight, 2) + std::pow(top.x - playerLeft, 2)));		
		float distanceBottom = std::fabs(std::sqrt(std::pow(bottom.x - playerRight, 2) + std::pow(bottom.x - playerLeft, 2)));		
		if (COLLISION_STAIR_RIGHT) { playerCheckStairs(bottom, top, true, false, isLeft); }
		else if (COLLISION_STAIR_RIGHT_TOP) { playerCheckStairs(bottom, top, false, true, isLeft); }
		else if (COLLISION_STAIR_RIGHT_TOP_EXIT) { playerCheckStairs(bottom, top, true, false, isLeft);
		}
	}
}

// check static blocks
void Collision::checkCollision(std::function<void(char c, float i, float s)> playerCollision) { 

	for (auto it : staticObjectPositions) {	
		// some hacky distance calculation - TODO: Refactor this.
		float blockCentreX = it.left + 16;
		float playerCentreX = playerLeft + 16;		
		float blockCentreY = it.bottom - 16;
		float playerCentreY = playerBottom - 16;
		float distanceX = std::fabsf(blockCentreX - playerCentreX);
		float distanceY = std::fabsf(playerCentreY - blockCentreY);

		if (distanceX <= 32 && distanceX >= 0 && distanceY <= 32 && distanceY >= 0) {
			if (COLLISION_BOTTOM) {
				playerCollision(BOTTOM, NO_CHANGE_GROUND_HEIGHT, 0);
				return;
			}
			if (COLLISION_LEFT) {
				playerCollision(LEFT, NO_CHANGE_GROUND_HEIGHT, 0);
				return;
			}
			else if (COLLISION_RIGHT) {
				playerCollision(RIGHT, NO_CHANGE_GROUND_HEIGHT, 0);
				return;
			}			
		}
	}
	
	for (auto it : staticPlatformPositions) {	 		
		float blockCentreX = it.left + 16;
		float playerCentreX = playerLeft + 16;	
		float blockCentreY = it.top - 16;
		float playerCentreY = playerBottom - 16;
		float distanceX = playerCentreX - blockCentreX;
		float distanceY = playerCentreY - blockCentreY;
		
		if (COLLISION_TOP) {
			playerCollision(TOP, it.top, 0);
			return;
		}
		
		else if (distanceX < -32 || distanceX > 32 || distanceY < 40) {
			playerCollision(NO_COLLISION, NO_CHANGE_GROUND_HEIGHT, 0);
			continue;
		}
		
		else {
			playerCollision(NO_COLLISION, NO_CHANGE_GROUND_HEIGHT, 0);
			continue;
		}
	}

	for (auto it : travelatorPositions) {
		float blockCentreX = it.left + 16;
		float playerCentreX = playerLeft + 16;
		float blockCentreY = it.top - 16;
		float playerCentreY = playerBottom - 16;
		float distanceX = playerCentreX - blockCentreX;
		float distanceY = playerCentreY - blockCentreY;
		float speed = it.speed;

		if (COLLISION_TOP) {
			playerCollision('�', it.top, speed);
			return;
		}

		else if (distanceX < -32 || distanceX > 32 || distanceY < 40) {
			playerCollision(NO_COLLISION, NO_CHANGE_GROUND_HEIGHT, 0);
			continue;
		}

		else {
			playerCollision(NO_COLLISION, NO_CHANGE_GROUND_HEIGHT, 0);
			continue;
		}
	}
}

void Collision::clearCollisionData() {
	staticObjectPositions.clear();
	staticPlatformPositions.clear();
	stairs.clear();
	entityPositions.clear();
}
