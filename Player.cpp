#include "Player.h"


Player::Player(int screenWidth, int screenHeight, std::string textureName) : 
	Sprite (screenWidth, screenHeight, textureName)
{
	input = std::unique_ptr<PlayerInput>(new PlayerInput());
	groundHeight = m_screenHeight - 50.0f;
	maxJumpHeight = m_screenHeight - 150.0f;
	setStartPosition();
}
 
void Player::setStartPosition()
{
	m_sprite.setPosition(sf::Vector2f(m_screenWidth / 2, groundHeight));
}

void Player::update(float dt)
{
	m_currentDirection = input->update(dt);
	checkMovement(dt);

	if (isJumping) {
		jump(dt, currentSpeed);
	}

}

void Player::checkMovement(float dt)
{
	switch (m_currentDirection){
	case 'l':
		moveHorizontal(dt, -PLAYER_SPEED);
		break;
	case 'r':
		moveHorizontal(dt, PLAYER_SPEED);
		break;
	case 'j':
		isJumping = true;
		break;
	case 's':
		isMoving = false;
		break;
	default:
		break;
	}
}

void Player::moveHorizontal(float dt, float speed)
{
	if (!isJumping) {
		m_sprite.move(sf::Vector2f(speed * dt, 0.0));
		currentSpeed = speed;
		isMoving = true;
	}
}

void Player::jump(float dt, float speed)
{
	std::cout << maxJumpHeight << std::endl;
	std::cout << m_sprite.getPosition().y;
	if (!isMoving) {
		if (!isAtMaxJumpHeight) {
			m_sprite.move(sf::Vector2f(0.0, -JUMP_SPEED * dt));
		}
		else if (isAtMaxJumpHeight) {
			m_sprite.move(sf::Vector2f(0.0, JUMP_SPEED * dt));
		}

		if (m_sprite.getPosition().y <= maxJumpHeight) {
			isAtMaxJumpHeight = true;					  			
		}

		if (m_sprite.getPosition().y >= groundHeight) {
			m_sprite.setPosition(sf::Vector2f(m_sprite.getPosition().x, groundHeight));
			isAtMaxJumpHeight = false;
			isJumping = false;	
		}				  		
	}



	/*
	if (isMoving) {								   
		m_sprite.move(sf::Vector2f(speed * dt, -0.0001 * dt));	
	}
	*/

}
