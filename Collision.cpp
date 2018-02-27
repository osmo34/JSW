#include "Collision.h"



Collision::Collision()
{
	playerLeft = 0.0f;
	playerRight = 0.0f;
	playerTop = 0.0f;
	playerBottom = 0.0f;
}						  

Collision::~Collision()	{ }

void Collision::updatePlayerPosition(std::function<double(char c)> position)
{
	playerLeft = position(LEFT);
	playerRight = position(RIGHT);
	playerTop = position(TOP);
	playerBottom = position(BOTTOM);
}

// only call once as static objects don't move
void Collision::updateStaticObjectPosition(std::function<double(char c)> position)
{								
	StaticObjectPositions m_staticObjectPosition;
	m_staticObjectPosition.top = position(TOP);
	m_staticObjectPosition.bottom = position(BOTTOM);
	m_staticObjectPosition.left = position(LEFT);
	m_staticObjectPosition.right = position(RIGHT);
	staticObjectPositions.push_back(m_staticObjectPosition);
}

void Collision::checkCollision(std::function<void(char c)> playerCollision)
{	
	for (auto it : staticObjectPositions) {				
		if (playerLeft <= it.right && 
			playerRight >= it.left && 
			playerTop >= it.top && playerTop) {		 
			if (playerTop >= it.bottom && playerBottom >= it.top) {
				return;
			}							 
			else if (playerLeft <= it.right && playerRight >= it.right)  {
				playerCollision(LEFT);
			}
			else if (playerRight >= it.left && playerLeft <= it.left) {
				playerCollision(RIGHT);
			}
		}
		else {
			playerCollision(NO_COLLISION);
		}
	}
}




