#include "Player.h"

Player::Player(int screenWidth, int screenHeight, sf::Texture texture) :
	Sprite (screenWidth, screenHeight, texture)	{
	input = std::unique_ptr<PlayerInput>(new PlayerInput());
	state = std::unique_ptr<PlayerState>(new PlayerState());
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
	checkState();
	state->updateState(NONE);
	animation->update(dt);

	if (isJumping) {
		jump(dt, currentSpeed);	
		onStairsLeft = false;
	}

	currentHeight = m_sprite.getPosition().y;	  
}

void Player::checkMovement(float dt) {
	const char LEFT = 'l', RIGHT = 'r', JUMP = 'j', STATIONARY = 's';

	switch (m_currentDirection) {
	case LEFT:
		if (!collideLeft) {
			moveHorizontal(dt, -PLAYER_SPEED);
			flipSprite(true);			
			animation->updateAnimation(dt, m_sourceRect, &m_sprite);
		}
		break;
	case RIGHT: 
		if (!collideRight) {
			moveHorizontal(dt, PLAYER_SPEED);			
			flipSprite(false);
			animation->updateAnimation(dt, m_sourceRect, &m_sprite);
		}
		break;
	case JUMP:		
		isJumping = true;
		animation->updateAnimation(dt, m_sourceRect, &m_sprite);
		break;
	case STATIONARY:
		isMoving = false;
		animation->updateAnimation(dt, m_sourceRect, &m_sprite);
		break;
	default:
		break;
	}
}

void Player::moveHorizontal(float dt, float speed) {

	(onStairsLeft) ? verticalSpeed = 20.0 : verticalSpeed = 0.0;

	if (!isJumping) {
		m_sprite.move(sf::Vector2f(speed * dt, verticalSpeed * speed));		
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
		else {
			m_sprite.move(sf::Vector2f(0.0, (JUMP_SPEED * m_grav) * dt));
			m_grav += GRAVITY_CALCULATION;
		}  
	}
	
	if (isMoving) {
		if (!isAtMaxJumpHeight) {
			m_sprite.move(sf::Vector2f(speed * dt, (-JUMP_SPEED * m_grav) * dt));
			m_grav -= GRAVITY_CALCULATION;
		}
		else {
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
	else {		
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
		NO_COLLISION = 'n', ENEMY = 'e', POWER_UP = 'p',
		STAIR_LEFT = 'z', STAIR_RIGHT = 'x', STAIR_NONE = 'c';
		
	if (currentHeight >= groundHeightOld + 1) {
		onStairsLeft = false;
	}	

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
	case STAIR_LEFT:
		onStairsLeft = true;
		break;
	case STAIR_RIGHT:
		break;
	case STAIR_NONE:
		onStairsLeft = false;
		break;				 
	case NO_COLLISION:
		collideLeft = false;
		collideRight = false;

		if (isJumping) {
			groundHeight = groundHeightOld;
		} 
		else {
			fall(deltaTime);
			fallCheck();
		} 
	default:
		break;		
	}	
}

void Player::updateGroundHeight(float gh) {
	
	if (!onStairsLeft) {
		if (gh == 0.0) {
			groundHeight = groundHeightOld;
			maxJumpHeight = maxJumpHeightOld;
		}
		else {
			groundHeight = gh;
			maxJumpHeight = gh - JUMP_HEIGHT;
		}
	}
	else {
		groundHeight = currentHeight;
		maxJumpHeight = gh - JUMP_HEIGHT;
	}
}

void Player::checkState() {
	const char DEAD = 'd', PICK_UP = 'u', NONE = 'n';
	char c = state->getState();

	switch (c) {
	case DEAD:
		std::cout << "dead" << std::endl;
		setStartPosition();
		break;
	case PICK_UP:
		std::cout << "pick up" << std::endl;
		break;
	case NONE:
	default:
		break;
	}
}

void Player::collisionEntity(bool isHarmful) {	
	(isHarmful) ? state->updateState(DEAD) : state->updateState(PICK_UP);
}



