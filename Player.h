#pragma once
#include "Sprite.h"
#include "PlayerInput.h"
class Player : public Sprite
{
private:
	void checkMovement(const float dt);
	void moveHorizontal(const float dt, const float speed);
	void jump(const float dt, const float speed);
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
	const float GRAVITY = 2.0;
	const float GRAVITY_CALCULATION = 0.04;
	float m_grav;


public:
	using Sprite::Sprite;	
	Player(int screenWidth, int screenHeight, std::string textureName);
	void update(const float dt);
	void setStartPosition();
};

