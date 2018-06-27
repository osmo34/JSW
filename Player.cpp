#include "Player.h"

//TODO: this is getting bloated - break up into smaller classes
//TODO: fix bug found when jumping from a really high platform - calls fall too many times

Player::Player(int screenWidth, int screenHeight, sf::Texture texture) :
	Sprite(screenWidth, screenHeight, texture) {
	input = std::unique_ptr<PlayerInput>(new PlayerInput());
	state = std::unique_ptr<PlayerState>(new PlayerState());
	groundHeightOld = groundHeight;
	m_grav = GRAVITY;
	resetPositionX = START_POSITION_X;
	resetPositionY = START_POSITION_Y;
	groundHeightPlatform = resetPositionY;
	setStartPosition();
	collideLeft = false;
	collideRight = false; 
}

void Player::setStartPosition() {
	m_sprite.setPosition(sf::Vector2f(resetPositionX, resetPositionY)); // TODO: temp to prevent falling through floor on first start (platform not loaded!)
	m_sprite.setOrigin(sf::Vector2f(0, 0));
}

void Player::update(float dt) {
	deltaTime = dt;
	m_currentDirection = input->update(deltaTime, isJumping);
	checkMovement(deltaTime);
	checkScreenEdge();
	checkState();
	animation->update(deltaTime);	

	if (isJumping) {
		jump(dt, currentSpeed);
		onStairsLeft = false;
		onStairsRight = false;
		topStairs = false;
	}
	checkStairs();
	checkFall();

	// Uncomment for debugging
	//std::cout << dt << std::endl;
	//std::cout << "collide right " << collideRight << std::endl;
	//std::cout << "jumping " << isJumping << std::endl;
	//updateFall();	// TODO - explore. Stairs left bug re-appears from time to times
	//std::cout << m_grav << std::endl;
	//std::cout << "gh " << groundHeight << std::endl;
	//std::cout << "player " << m_sprite.getPosition().y << std::endl;
	//std::cout << onStairsLeft << std::endl;
	//std::cout << groundHeightPlatform << std::endl;
	//std::cout << landed << std::endl;
	//std::cout << state->getState() << std::endl;
	//std::cout << groundHeightPlatform << std::endl;
	//std::cout << lastPositionY << std::endl;
	//std::cout << "is falling " << isFalling << std::endl;
	//std::cout << lastPositionY << std::endl;
	//std::cout << collideBottom << std::endl;
}

void Player::checkMovement(float dt) {
	const char LEFT = 'l', RIGHT = 'r', JUMP = 'j', STATIONARY = 's';
	
	switch (m_currentDirection) {
	case LEFT:
		if (!collideLeft) {
			moveHorizontal(dt, -PLAYER_SPEED);
			flipSprite(true);
			animation->updateAnimation(dt, m_sourceRect, &m_sprite, isflipLeft);
		}
		break;
	case RIGHT:
		if (!collideRight) {
			moveHorizontal(dt, PLAYER_SPEED);
			flipSprite(false);
			animation->updateAnimation(dt, m_sourceRect, &m_sprite, isflipLeft);
		}
		break;
	case JUMP:
		if (!isJumping && !isFalling && !collideBottom) {
			isJumping = true;
			state->updateState(JUMP);
			animation->updateAnimation(dt, m_sourceRect, &m_sprite, isflipLeft);
		}
		break;
	case STATIONARY:
		moveHorizontal(dt, 0.0f);
		break;
	default:
		break;
	}
}

void Player::moveHorizontal(float dt, float speed) {
	currentSpeed = speed;
	if (!isJumping) {
		if (onStairsLeft)
			m_sprite.move(sf::Vector2f(verticalSpeed * speed * dt, verticalSpeed * speed * dt));
		else if (onStairsRight)
			m_sprite.move(sf::Vector2f(verticalSpeed * speed * dt, verticalSpeed * -speed * dt));
		else {
			m_sprite.setPosition(sf::Vector2f((m_sprite.getPosition().x) + speed * dt, m_sprite.getPosition().y));	
		}
	}
}

void Player::jump(float dt, float speed) {
	landed = false;
	if (!collideRight && !collideLeft) {
		m_sprite.move(sf::Vector2f(speed * dt, (-JUMP_SPEED * m_grav) * dt));
		m_grav -= std::sin((GRAVITY_CALCULATION) * dt);
	} 
	else if (collideRight || collideLeft) {
		currentSpeed = 0;
		m_sprite.move(sf::Vector2f(speed * dt, (-JUMP_SPEED * m_grav) * dt));
		m_grav -= std::sin((GRAVITY_CALCULATION) * dt);
	}	
	fallCheck();
}

void Player::fall(float dt) {	
	m_sprite.setPosition(sf::Vector2f(m_sprite.getPosition().x, (m_sprite.getPosition().y) + (FALL_SPEED * dt)));
}

void Player::fallCheck() {	
	const char NONE = 'n';
	if (m_sprite.getPosition().y > groundHeight) {
		m_sprite.setPosition(sf::Vector2f(m_sprite.getPosition().x, groundHeight));
		updateGroundHeight(0.0);
		m_grav = GRAVITY;
		isAtMaxJumpHeight = false;
		isJumping = false;

		// Related to sound effects only at this time (prevents replaying, odd level changing behaviour)
		if (!landed) {
			state->updateState(NONE);
			landed = true;
		}
	}
}

void Player::collision(char c, float gh, float speed) {
	const char LEFT = 'l', RIGHT = 'r', TOP = 't', BOTTOM = 'b',
		NO_COLLISION = 'n', ENEMY = 'e', POWER_UP = 'p',
		STAIR_LEFT = 'z', STAIR_RIGHT = 'x', STAIR_NONE = 'c', TRAVELATOR = '£';;

	if (gh == 0.0) { gh = groundHeightOld; }
	switch (c) {
	case LEFT:
		collideBottom = false;
		collideLeft = true;
		collideRight = false;
		groundHeight = groundHeightPlatform;
		break;
	case RIGHT:
		collideBottom = false;
		collideRight = true;
		collideLeft = false;
		groundHeight = groundHeightPlatform;
		break;
	case TOP:
		collideBottom = false;
		collideLeft = false;
		collideRight = false;
		updateGroundHeight(gh);
		groundHeightPlatform = gh;
		m_sprite.setPosition(sf::Vector2f(m_sprite.getPosition().x, gh));
		lastPositionY = m_sprite.getPosition().y;
		break;
	case BOTTOM:
		collideBottom = true;
		isJumping = false;		
		isFalling = true;
		m_grav = GRAVITY;
		fall(deltaTime);
		state->updateState(NONE);
		break;
	case TRAVELATOR:
		collideBottom = false;
		collideLeft = false;
		collideRight = false;
		updateGroundHeight(gh);
		groundHeightPlatform = gh;
		m_sprite.setPosition(sf::Vector2f(m_sprite.getPosition().x, gh));
		m_sprite.move(-1.5f, 0); // TODO: just a test, we will of course use speed
		lastPositionY = m_sprite.getPosition().y;		
		//std::cout << "TEST here" << std::endl;
		break;
	case NO_COLLISION:
		if (!onStairsLeft && !onStairsRight) {
			if (topStairs) {
				topStairs = false;
				break;
			}
			else if (isJumping) { 
				groundHeight = groundHeightOld;
			}
			else if (!isFalling) {			
				fall(0.01 * deltaTime);	// hopefully prevent jolting on initial fall
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
	}
	else {
		groundHeight = gh;
	}
}

void Player::checkState() {
	const char DEAD = 'd', PICK_UP = 'u', NONE = 'n', LEFT = 'l', RIGHT = 'r', DOWN = 'D', UP = 'U';
	char c = state->getState();
	auto resetStartPosition = [&]() { resetPositionX = m_sprite.getPosition().x; resetPositionY = m_sprite.getPosition().y; }; // reset position
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
		m_sprite.setPosition(sf::Vector2f(m_screenWidth - 50, m_sprite.getPosition().y));
		resetStartPosition(); 
		break;
	case RIGHT:
		m_sprite.setPosition(sf::Vector2f(20, m_sprite.getPosition().y));
		resetStartPosition();
		break;
	case DOWN:		
		m_sprite.setPosition(sf::Vector2f(m_sprite.getPosition().x, 32));
		lastPositionY = 0;
		resetStartPosition();
		break;
	case UP:
		m_sprite.setPosition(sf::Vector2f(m_sprite.getPosition().x, 664)); // TODO: Hacky
		isJumping = false;
		isFalling = false;
		updateGroundHeight(0.0);
		resetStartPosition();
		break;
	case NONE:
		//state->updateState(NONE);
		break;
	default:
		break;
	}
}

void Player::collisionEntity(bool isHarmful) {
	(isHarmful) ? state->updateState(DEAD) : state->updateState(PICK_UP);
}

void Player::checkScreenEdge() {
	const char LEFT = 'l', RIGHT = 'r', UP = 'U', DOWN = 'D';
	if (m_sprite.getPosition().x < 0.0) {
		state->updateState(LEFT);
	}
	else if (m_sprite.getPosition().x > m_screenWidth) {
		state->updateState(RIGHT);
	}
	else if (m_sprite.getPosition().y >= groundHeightOld) {
		state->updateState(DOWN);			
	}
	else if (m_sprite.getPosition().y <= 32) {
		state->updateState(UP);
	}
}

void Player::checkStairs() {
	const float pixelOffset = 6.0f;

	if (onStairsLeft) {
		if (m_sprite.getPosition().x >= currentStairsBottom.x + 32) {
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

// check if falling and if so disable jumping
void Player::checkFall() {	
	if (verticalSpeed > 0) { // make sure this only works if we are not on stairs
		isFalling = false;
		return;
	}
	(m_sprite.getPosition().y > lastPositionY) ? isFalling = true : isFalling = false;
	// make character fall
	if (isFalling) {
		fall(deltaTime);
	}

}

void Player::onStairs(sf::Vector2f bottom, sf::Vector2f top, bool onStairsBottom, bool onStairsTop, bool isStairsLeft) {
	const float angle = std::fabs(atan2(top.y - bottom.y, top.x - bottom.x) * PI / 180);
	(isStairsLeft) ?
		updateStairs(onStairsLeft, onStairsBottom, onStairsTop, bottom, top, std::cos(angle)) :
		updateStairs(onStairsRight, onStairsBottom, onStairsTop, bottom, top, std::cos(angle));
}

char Player::externalCheckState() {
	return state->getState();
}

void Player::externalResetState() {
	state->updateState(NONE);
}

