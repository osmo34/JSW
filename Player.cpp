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

	// test code
	/*
	float left = getCollision('l');
	float right = getCollision('r');
	float top = getCollision('t');
	float bottom = getCollision('b');	
	std::cout << "left " << left << std::endl;
	std::cout << "Right " << right << std::endl;
	std::cout << "Top " << top << std::endl;
	std::cout << "Bottom " << bottom << std::endl;
	*/						  
}

void Player::checkMovement(float dt) {
	switch (m_currentDirection) {
	case 'l':
		if (!collideLeft) {
			moveHorizontal(dt, -PLAYER_SPEED);
		}
		break;
	case 'r': 
		if (!collideRight) {
			moveHorizontal(dt, PLAYER_SPEED);
		}
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
		isAtMaxJumpHeight = true;
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
	switch (c) {
	case 'l':
		collideLeft = true;
		collideRight = false;
		isMoving = false;
		break;
	case 'r':
		collideRight = true;
		collideLeft = false;
		isMoving = false;
		break;
	case 't':
		collideLeft = false;
		collideRight = false;
		updateGroundHeight(gh);
		break;
	case 'b':
		break;
	case 'n':		
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



