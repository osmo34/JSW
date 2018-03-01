#include "Collision.h"
						   
Collision::Collision() {
	playerLeft = 0.0f;
	playerRight = 0.0f;
	playerTop = 0.0f;
	playerBottom = 0.0f;
}						  

Collision::~Collision()	{ }

void Collision::updatePlayerPosition(std::function<double(char c)> position) {
	playerLeft = position(LEFT);
	playerRight = position(RIGHT);
	playerTop = position(TOP);
	playerBottom = position(BOTTOM);	 	
}

// only call once as static objects don't move
void Collision::updateStaticObjectPosition(std::function<double(char c)> position) {								
	StaticObjectPositions m_staticObjectPosition;
	m_staticObjectPosition.top = position(TOP);
	m_staticObjectPosition.bottom = position(BOTTOM);
	m_staticObjectPosition.left = position(LEFT);
	m_staticObjectPosition.right = position(RIGHT);
	staticObjectPositions.push_back(m_staticObjectPosition);
}

void Collision::checkCollision(std::function<void(char c, float i)> playerCollision) {	
	
	// TODO: major bug to be fixed with higher platforms

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




