#pragma once
#include "Sprite.h"
#include "PlayerInput.h"
class Player : public Sprite
{
private:
	void move(float dt);
	char m_currentDirection;
	std::unique_ptr <PlayerInput> input;
	const float m_playerSpeed = 0.001;

public:
	using Sprite::Sprite;	
	Player(int screenWidth, int screenHeight, std::string textureName);

	void update(float dt);
	void setStartPosition();
};

