#pragma once
#include "Sprite.h"
#include "PlayerInput.h"
class Player : public Sprite
{
private:

	void checkMovement(const float dt);
	void moveHorizontal(const float dt, const float speed);
	void jump(const float dt, const float speed);
	void fall(const float dt);
	void fallCheck();

	std::unique_ptr <PlayerInput> input;

	char m_currentDirection;			
	const float PLAYER_SPEED = 0.0001f;
	const float JUMP_SPEED = 0.0001f;
	const float JUMP_HEIGHT = 100.0f;
	float currentSpeed;
	float groundHeight;
	float groundHeightOld;
	float maxJumpHeight;
	float maxJumpHeightOld;
	bool isMoving = false;
	bool isJumping = false;
	bool isAtMaxJumpHeight = false;
	const float GRAVITY = 2.0;
	const float GRAVITY_CALCULATION = 0.04f;
	float m_grav;
	bool collideLeft;
	bool collideRight;
	float deltaTime;

public:
	const char objectId = 'p';
	using Sprite::Sprite;	
	Player(const int screenWidth, const int screenHeight, sf::Texture texture);
	void update(const float dt);
	void setStartPosition();
	void collision(char c, float gh);
	void updateGroundHeight(float gh);
	void killPlayer();
};

