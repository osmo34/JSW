#pragma once
#include <iostream>
#include <functional>
#include <vector>

struct StaticObjectPositions {
	float top;
	float bottom;
	float left;
	float right;
};

class Collision
{
private:
	float playerLeft;
	float playerRight;
	float playerTop;
	float playerBottom;

	const char LEFT = 'l';
	const char RIGHT = 'r';
	const char TOP = 't';
	const char BOTTOM = 'b';
	const char NO_COLLISION = 'n';

	const float COLLISION_OFFSET = 15.0f; // offset for landing
	const float NO_CHANGE_GROUND_HEIGHT = 0.0f;

	std::vector<StaticObjectPositions> staticObjectPositions;
	
public:
	Collision();
	~Collision();

	void updatePlayerPosition(std::function<double(char c)> position);
	void updateStaticObjectPosition(std::function<double(char c)> position);
	void checkCollision(std::function<void(char c, float i)> playerCollision);
};

