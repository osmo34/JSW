#include "Player.h"

Player::Player(int screenWidth, int screenHeight, sf::Texture texture) :
	Sprite (screenWidth, screenHeight, texture)	{
	input = std::unique_ptr<PlayerInput>(new PlayerInput());
	groundHeight = m_screenHeight - 50.0f;
	groundHeightOld = groundHeight;
	maxJumpHeight = groundHeight - JUMP_HEIGHT;
	maxJumpHeightOld = maxJumpHeight;
	m_grav = GRAVITY;
	setStartPosition();
	collideLeft = false;
	collideRight = false;
}
 
void Player::setStartPosition() {
	m_sprite.setPosition(sf::Vector2f(m_screenWidth / 2, groundHeight));
	m_sprite.setOrigin(sf::Vector2f(0, 0));
}

void Player::update(float dt) {
	deltaTime = dt;
	m_currentDirection = input->update(dt);
	checkMovement(dt);

	if (isJumping) {
		jump(dt, currentSpeed);
	}

	/* test code
	std::cout << "**********" << std::endl;
	std::cout << std::endl;
	std::cout << "sprite pos " << m_sprite.getPosition().y << std::endl;
	std::cout << "groundheight " << groundHeight << std::endl;
	std::cout << "jumpheight " << maxJumpHeight << std::endl;
	std::cout << "grav " << m_grav << std::endl;
	std::cout << "current speed " << currentSpeed << std::endl;
	std::cout << "is moving " << isMoving << std::endl;
	std::cout << "is jumping " << isJumping << std::endl;
	std::cout << "is at max jump height " << isAtMaxJumpHeight << std::endl;
	std::cout << "**********" << std::endl;
	std::cout << std::endl;
	*/
}

void Player::checkMovement(float dt) {
	const char LEFT = 'l', RIGHT = 'r', JUMP = 'j', STATIONARY = 's';

	switch (m_currentDirection) {
	case LEFT:
		if (!collideLeft) {
			moveHorizontal(dt, -PLAYER_SPEED);
		}
		break;
	case RIGHT: 
		if (!collideRight) {
			moveHorizontal(dt, PLAYER_SPEED);
		}
		break;
	case JUMP:
		isJumping = true;
		break;
	case STATIONARY:
		isMoving = false;
		break;
	default:
		break;
	}
}

void Player::moveHorizontal(float dt, float speed) {
	if (!isJumping) {
		m_sprite.move(sf::Vector2f(speed * dt, 0.0));
		currentSpeed = speed;
		isMoving = true;
	}
}

void Player::jump(float dt, float speed) {	
	if (!isMoving) {
		if (!isAtMaxJumpHeight) {
			m_sprite.move(sf::Vector2f(0.0, (-JUMP_SPEED * m_grav) * dt));
			m_grav -= GRAVITY_CALCULATION;
		}
		else if (isAtMaxJumpHeight) {
			m_sprite.move(sf::Vector2f(0.0, (JUMP_SPEED * m_grav) * dt));
			m_grav += GRAVITY_CALCULATION;
		}  
	}
	
	if (isMoving) {
		if (!isAtMaxJumpHeight) {
			m_sprite.move(sf::Vector2f(speed * dt, (-JUMP_SPEED * m_grav) * dt));
			m_grav -= GRAVITY_CALCULATION;
		}
		else if (isAtMaxJumpHeight) {
			m_sprite.move(sf::Vector2f(speed * dt, (JUMP_SPEED * m_grav) * dt));
			m_grav += GRAVITY_CALCULATION;
		}
	}
	fallCheck(); 
} 

void Player::fall(const float dt) {
	if (!isMoving) {
		m_sprite.move(sf::Vector2f(0.0, (JUMP_SPEED * m_grav) * dt));
		m_grav += GRAVITY_CALCULATION;
	}
	if (isMoving) {		
		m_sprite.move(sf::Vector2f(currentSpeed, (JUMP_SPEED * m_grav) * dt));
		m_grav += GRAVITY_CALCULATION;
	}
}

void Player::fallCheck() {	
	if (m_sprite.getPosition().y <= maxJumpHeight) {		
		m_grav = GRAVITY;
	}

	if (m_sprite.getPosition().y >= groundHeight) {
		m_sprite.setPosition(sf::Vector2f(m_sprite.getPosition().x, groundHeight));				
		updateGroundHeight(0.0);
		m_grav = GRAVITY;
		isAtMaxJumpHeight = false;
		isJumping = false;
	}
} 

void Player::collision(char c, float gh) {
	const char LEFT = 'l', RIGHT = 'r', TOP = 't', BOTTOM = 'b', 
		NO_COLLISION = 'n', ENEMY = 'e', POWER_UP = 'p';

	switch (c) {
	case LEFT:
		collideLeft = true;
		collideRight = false;
		isMoving = false;
		break;
	case RIGHT:
		collideRight = true;
		collideLeft = false;
		isMoving = false;
		break;
	case TOP:	
		collideLeft = false;
		collideRight = false;
		updateGroundHeight(gh);
		m_sprite.setPosition(sf::Vector2f(m_sprite.getPosition().x, gh));
		break;
	case BOTTOM:
		isJumping = false;
		fall(deltaTime);
		fallCheck();
		break;
	case ENEMY:
		break;
	case POWER_UP:
		break;
	case NO_COLLISION:
		collideLeft = false;
		collideRight = false;
		if (isJumping) {
			groundHeight = groundHeightOld;
		}
		else if (!isJumping) {
			fall(deltaTime);
			fallCheck();
		} 
	default:
		break;		
	}	
}

void Player::updateGroundHeight(float gh) {
	if (gh == 0.0) {		
		groundHeight = groundHeightOld;
		maxJumpHeight = maxJumpHeightOld;
	}
	else {
		groundHeight = gh;
		maxJumpHeight = gh - JUMP_HEIGHT;
	}
}



