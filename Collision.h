#pragma once
#include <iostream>
#include <functional>
#include <vector>

struct ObjectPositions {
	float top;
	float bottom;
	float left;
	float right;
};

class Collision
{
private:
	// used for non-player items and update accordingly
	template <typename T>
	void updatePositions(std::vector<T> t, ObjectPositions m_objectPosition) {
		t.clear();
		t.shrink_to_fit();
		t.push_back(m_objectPosition);
	}

	float playerLeft, playerRight, playerTop, playerBottom;

	const char LEFT = 'l', RIGHT = 'r', TOP = 't', BOTTOM = 'b', NO_COLLISION = 'n';

	const float COLLISION_OFFSET = 15.0f; // offset for landing
	const float NO_CHANGE_GROUND_HEIGHT = 0.0f;

	std::vector<ObjectPositions> staticObjectPositions;
	std::vector<ObjectPositions> movingEnemyPositions;
	
public:
	Collision();
	~Collision();

	void updateObjectPosition(std::function<double(char c)> position, char t);
	void checkCollision(std::function<void(char c, float i)> playerCollision);
};

