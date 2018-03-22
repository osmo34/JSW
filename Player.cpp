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
	m_sprite.setPosition(sf::Vector2f(350, groundHeight));
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
		if (!onStairsLeft) {
			collideLeft = false;
			collideRight = false;

			if (isJumping) {
				groundHeight = groundHeightOld;
			}
			else {
				fall(deltaTime);
				fallCheck();
			}
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
 
void Player::checkStairs() {
	if (onStairsLeft) {
		if (m_sprite.getPosition().x >= currentStairsBottom.x + 6) {  // TODO: reminder random variables
			onStairsLeft = false;
			currentStairsBottom = sf::Vector2f(0, 0);
			currentStairsTop = sf::Vector2f(0, 0);
		}
	}
	else {
		currentStairsBottom = sf::Vector2f(0, 0);
		currentStairsTop = sf::Vector2f(0, 0);
		verticalSpeed = 0.0f;
	}
}

void Player::calculateVerticalSpeed(float angle, float distance) {		
	verticalSpeed = std::sin(angle) * distance;								
}

void Player::onStairs(sf::Vector2f bottom, sf::Vector2f top)
{
	if (!onStairsLeft) {
		onStairsLeft = true;
		currentStairsBottom = bottom;
		currentStairsTop = top;

		// get the angle and distance between 2 points. Divides distance by 100 to speed the player up vertically
		float angle = std::fabs(atan2(top.y - bottom.y, top.x - bottom.x) * 180 / PI);
		float distance = std::fabs(std::sqrt(std::pow(bottom.x - top.x, 2) + std::pow(bottom.y - top.y, 2))) / 100;
		calculateVerticalSpeed(angle, distance);
	}
}



