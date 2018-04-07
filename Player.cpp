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
	m_sprite.setPosition(sf::Vector2f(800, groundHeight));
	m_sprite.setOrigin(sf::Vector2f(0, 0));
}

void Player::update(float dt) {
	deltaTime = dt;
	m_currentDirection = input->update(dt);
	checkMovement(dt);
	checkScreenEdge();
	checkState();
	//state->updateState(NONE);
	animation->update(dt);

	if (isJumping) {
		jump(dt, currentSpeed);	
		onStairsLeft = false;
		onStairsRight = false;
		topStairs = false;
	}

	currentHeight = m_sprite.getPosition().y;
	checkStairs();
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
		
	if (!isJumping) {		
		if (onStairsLeft)			
			m_sprite.move(sf::Vector2f(verticalSpeed * speed * dt, verticalSpeed * speed * dt));
		else if (onStairsRight)
			m_sprite.move(sf::Vector2f(verticalSpeed * speed * dt, verticalSpeed * -speed * dt));
		else
			m_sprite.move(sf::Vector2f(speed * dt, 0.0));

		currentSpeed = speed;
		groundHeight = m_sprite.getPosition().y;
		updateGroundHeight(groundHeight);
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
		// divide by 10 to stop near instant falling
		if (!isMoving) {
			m_sprite.move(sf::Vector2f(0.0, (JUMP_SPEED * m_grav / 10) * dt));
			m_grav += GRAVITY_CALCULATION;
		}
		else {
			m_sprite.move(sf::Vector2f(currentSpeed, (JUMP_SPEED * m_grav / 10) * dt));
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
	case NO_COLLISION:
		if (!onStairsLeft &&!onStairsRight) {
			collideLeft = false;
			collideRight = false;

			if (topStairs) {
				updateGroundHeight(currentHeight);
				m_sprite.setPosition(sf::Vector2f(m_sprite.getPosition().x, currentHeight));
				break;
			}
			if (isJumping) {
				groundHeight = groundHeightOld;
			}
			else {
				fall(deltaTime);
				fallCheck();
			}
		}
		break;
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

void Player::checkState() {
	const char DEAD = 'd', PICK_UP = 'u', NONE = 'n', LEFT = 'l', RIGHT = 'r';
	char c = state->getState();

	switch (c) {
	case DEAD:
		std::cout << "dead" << std::endl;
		setStartPosition();
		state->updateState(NONE);
		break;
	case PICK_UP:
		std::cout << "pick up" << std::endl;
		state->updateState(NONE);
		break;
	case LEFT:
		m_sprite.setPosition(sf::Vector2f(m_screenWidth, m_sprite.getPosition().y));
		break;
	case RIGHT:
		m_sprite.setPosition(sf::Vector2f(0, m_sprite.getPosition().y));
		break;
	case NONE:
	default:
		break;
	}
}

void Player::collisionEntity(bool isHarmful) {	
	(isHarmful) ? state->updateState(DEAD) : state->updateState(PICK_UP);
}
  
void Player::checkScreenEdge()
{
	const char LEFT = 'l', RIGHT = 'r';
	if (m_sprite.getPosition().x < 0.0) {
		state->updateState(LEFT);
	}
	else if (m_sprite.getPosition().x > m_screenWidth) {
		state->updateState(RIGHT);
	}
}

 
void Player::checkStairs() {
	const float pixelOffset = 6.0f;
	
	if (onStairsLeft) {	
		if (m_sprite.getPosition().x >= currentStairsBottom.x + pixelOffset) {
			onStairsLeft = false;
		}	
	}  	
	else if (onStairsRight) {
		if (m_sprite.getPosition().x <= currentStairsBottom.x - 32) { // TODO: Pixel size magic number
			onStairsRight = false;
		}
	} 
	else if (topStairs) {  		
		if (m_sprite.getPosition().x <= currentStairsTop.x - pixelOffset) {
			// fakes a tiny jump so we land back on the block - stops a weird wobble
			m_grav = 0.001;
			isJumping = true;			
			topStairs = false;
		}
	} 
	else if (!onStairsLeft && !onStairsRight) {
		currentStairsBottom = sf::Vector2f(0, 0);
		currentStairsTop = sf::Vector2f(0, 0);
		verticalSpeed = 0.0f;
	}
}

void Player::updateStairs(bool &stairs, bool &stairsBottom, bool &stairsTop, sf::Vector2f bottom, sf::Vector2f top, float vs) {
	if (!stairs && stairsBottom) {
		topStairs = false;
		stairs = true;
		currentStairsBottom = bottom;
		currentStairsTop = top;		
		verticalSpeed = vs;
	}
	else if (stairs && stairsTop) {
		topStairs = true;
		stairs = false;
		verticalSpeed = 0.0f;
	}
}

float Player::calculateVerticalSpeed(float distance, float angle) {
	return angle * distance;
}

void Player::onStairs(sf::Vector2f bottom, sf::Vector2f top, bool onStairsBottom, bool onStairsTop, bool isStairsLeft) {
	const float angle = std::fabs(atan2(top.y - bottom.y, top.x - bottom.x) * 180 / PI);
	const float distance = std::fabs(std::sqrt(std::pow(bottom.x - top.x, 2) + std::pow(bottom.y - top.y, 2))) / 100;
	(isStairsLeft) ? 
		updateStairs(onStairsLeft, onStairsBottom, onStairsTop, bottom, top, calculateVerticalSpeed(distance, std::sin(angle))) : 
		updateStairs(onStairsRight, onStairsBottom, onStairsTop, bottom, top, calculateVerticalSpeed(distance, std::cos(angle)));	
}

char Player::externalCheckState() {
	return state->getState();
}

void Player::externalResetState()
{
	state->updateState(NONE);
}



