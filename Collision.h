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

	std::vector<StaticObjectPositions> staticObjectPositions;

public:
	Collision();
	~Collision();

	void updatePlayerPosition(std::function<double(char c)> position);
	void updateStaticObjectPosition(std::function<double(char c)> position);
};

