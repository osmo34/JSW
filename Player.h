#pragma once
#include "Sprite.h"
#include "PlayerInput.h"
class Player : public Sprite
{
private:
	void checkMovement(float dt);
	void moveHorizontal(float dt, float speed);
	void jump(float dt, float speed);
	char m_currentDirection;
	std::unique_ptr <PlayerInput> input;
	const float PLAYER_SPEED = 0.0001;
	const float JUMP_SPEED = 0.0001;
	float currentSpeed;
	float groundHeight;
	float maxJumpHeight;
	bool isMoving = false;
	bool isJumping = false;
	bool isAtMaxJumpHeight = false;


public:
	using Sprite::Sprite;	
	Player(int screenWidth, int screenHeight, std::string textureName);

	void update(float dt);
	void setStartPosition();
};

