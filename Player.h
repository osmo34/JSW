#pragma once
#include "Sprite.h"
#include "PlayerInput.h"
#include "PlayerState.h"
#include "MediaPlayer.h"
#include <cmath>

#define PI 3.14159265359

class Player : public Sprite
{
private:
	void checkMovement(const float dt);
	void moveHorizontal(const float dt, const float speed);
	void jump(const float dt, const float speed);
	void fall(float dt);
	void fallCheck();
	void checkState();
	void checkScreenEdge();

	std::unique_ptr <PlayerInput> input = nullptr;
	std::unique_ptr <PlayerState> state = nullptr;
	const char DEAD = 'd', PICK_UP = 'u', NONE = 'n';

	char m_currentDirection;
	const float PLAYER_SPEED = 0.17f;
	const float STAIR_SPEED = 1.0f;
	bool onStairsLeft = false;
	bool onStairsRight = false;
	float verticalSpeed = 0.0f;
	float currentHeight;
	const float JUMP_SPEED = 0.1f;
	const float JUMP_HEIGHT = 100.0f;
	bool landed = true;
	float currentSpeed;
	float groundHeight = 764.0f;
	float groundHeightOld;
	float groundHeightPlatform; // fix to retain ground height when jumping into walls when on a platform
	float maxJumpHeight;
	float maxJumpHeightOld;
	bool isMoving = false;
	bool isJumping = false;
	bool isAtMaxJumpHeight = false;
	const float GRAVITY = 2.0;
	const float GRAVITY_CALCULATION = 0.004f;
	const float FALL_SPEED = 0.25f;
	float m_grav;
	bool collideLeft;
	bool collideRight;
	float deltaTime;
	sf::Vector2f currentStairsBottom;
	sf::Vector2f currentStairsTop;
	void checkStairs();
	float horiztonalSpeed = 1.0f;
	bool topStairs = false;
	bool canFall = true; // TODO - change to false on stairs
	bool canJump = true;

	bool collideBottom = false;

	void updateStairs(bool &stairs, bool &stairsBottom, bool &stairsTop, sf::Vector2f bottom, sf::Vector2f top, float vs);

	// prevent jumping whilst falling
	float lastPositionY;
	bool isFalling = false;
	void checkFall();

	const float START_POSITION_X = 950;
	const float START_POSITION_Y = 606;
	float resetPositionX;
	float resetPositionY;

	bool isFirstRun = true;

public:
	const char objectId = 'p';
	using Sprite::Sprite;
	Player(const int screenWidth, const int screenHeight, sf::Texture texture);
	void update(const float dt);
	void setStartPosition();
	void collision(char c, float gh, float speed);
	void updateGroundHeight(float gh);
	void collisionEntity(bool isHarmful);
	void onStairs(sf::Vector2f bottom, sf::Vector2f top, bool onStairsBottom, bool onStairsTop, bool isStairsLeft);
	char externalCheckState();
	void externalResetState();
};