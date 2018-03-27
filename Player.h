#pragma once
#include "Sprite.h"
#include "PlayerInput.h"
#include "PlayerState.h"
#include <cmath>

#define PI 3.141

class Player : public Sprite
{
private:

	void checkMovement(const float dt);
	void moveHorizontal(const float dt, const float speed);
	void jump(const float dt, const float speed);
	void fall(const float dt);
	void fallCheck();
	void checkState();

	std::unique_ptr <PlayerInput> input = nullptr;
	std::unique_ptr <PlayerState> state = nullptr;
	const char DEAD = 'd', PICK_UP = 'u', NONE = 'n';

	char m_currentDirection;			
	const float PLAYER_SPEED = 0.1f;
	bool onStairsLeft = false;
	bool onStairsRight = false;
	float verticalSpeed = 0.0f;
	float currentHeight;
	const float JUMP_SPEED = 0.1f;
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
	sf::Vector2f currentStairsBottom;
	sf::Vector2f currentStairsTop;	
	void checkStairs();																	 
	float horiztonalSpeed = 1.0f; 
	bool topStairs = false;	
	float calculateVerticalSpeed(float distance, float angle);
	void updateStairs(bool &stairs, bool &stairsBottom, bool &stairsTop, sf::Vector2f bottom, sf::Vector2f top, float vs);

public:
	const char objectId = 'p';
	using Sprite::Sprite;	
	Player(const int screenWidth, const int screenHeight, sf::Texture texture);
	void update(const float dt);
	void setStartPosition();
	void collision(char c, float gh);
	void updateGroundHeight(float gh);
	void collisionEntity(bool isHarmful);	
	void onStairs(sf::Vector2f bottom, sf::Vector2f top, bool onStairsBottom, bool onStairsTop, bool isStairsLeft);
};

