#include "Player.h"


Player::Player(int screenWidth, int screenHeight, std::string textureName) : 
	Sprite (screenWidth, screenHeight, textureName)
{
	input = std::unique_ptr<PlayerInput>(new PlayerInput());
	setStartPosition();
}
 
void Player::setStartPosition()
{
	m_sprite.setPosition(sf::Vector2f(m_screenWidth / 2, m_screenHeight - 50.0));
}

void Player::update(float dt)
{
	m_currentDirection = input->update(dt);
	move(dt);
}

void Player::move(float dt)
{
	switch (m_currentDirection){
	case 'l':
		m_sprite.move(sf::Vector2f(-m_playerSpeed * dt, 0.0));
		break;
	case 'r':
		m_sprite.move(sf::Vector2f(m_playerSpeed * dt, 0.0));
		break;
	case 'j':
		break;
	default:
		break;
	}
}
