#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <math.h>
#include <SFML\Graphics.hpp>

struct ObjectPositions {
	float top;
	float bottom;
	float left;
	float right;
};

class Collision
{
private:
	
	sf::RectangleShape updatePositions(std::vector<ObjectPositions> &t, ObjectPositions &m_objectPosition, sf::RectangleShape collisionBox);
	void updatePositions(ObjectPositions m_objectPositions);

	const char LEFT = 'l', RIGHT = 'r', TOP = 't', BOTTOM = 'b', NO_COLLISION = 'n';
	const float COLLISION_OFFSET = 15.0f; // offset for landing
	const float NO_CHANGE_GROUND_HEIGHT = 0.0f;

	float playerLeft, playerRight, playerTop, playerBottom;

	std::vector<ObjectPositions> staticObjectPositions;
	std::vector<ObjectPositions> movingEnemyPositions;

	sf::RectangleShape playerRectangle;
	sf::RectangleShape enemyRectangle;


	
public:
	Collision();
	~Collision();

	void updateObjectPosition(std::function<double(char c)> position, char t);
	void checkCollision(std::function<void(char c, float i)> playerCollision);

	bool checkCollision();

	// temp
	void draw(sf::RenderWindow *window);
};

