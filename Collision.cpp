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
	float playerLeft = position(LEFT);
	float playerRight = position(RIGHT);
	float playerTop = position(TOP);
	float playerBottom = position(BOTTOM);
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


