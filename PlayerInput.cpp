#include "PlayerInput.h"

PlayerInput::PlayerInput() : m_currentDirection(STATIONARY)
{
}	  

PlayerInput::~PlayerInput(){}	   

void PlayerInput::keyboadInput(float dt)
{	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		m_currentDirection = LEFT;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		m_currentDirection = RIGHT;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		m_currentDirection = JUMP;
	}
	else {
		m_currentDirection = STATIONARY;
	}
}	   

char PlayerInput::update(float dt)
{
	keyboadInput(dt);
	return m_currentDirection;
}
