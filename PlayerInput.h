#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>

class PlayerInput
{
private:
	void keyboadInput(const float dt);
	char m_currentDirection;
	const char JUMP = 'j';
	const char LEFT = 'l';
	const char RIGHT = 'r';	
	const char STATIONARY = 's';


public:
	PlayerInput();
	~PlayerInput();
	char update(const float dt);


};

