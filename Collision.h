#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <math.h>
#include <SFML\Graphics.hpp>

// macros for collision
// collision static objects
#define COLLISION_TOP playerBottom >= it.top && playerBottom < it.top + COLLISION_OFFSET && playerLeft <= it.right && playerRight >= it.left
#define COLLISION_NONE playerTop >= it.bottom && playerBottom >= it.top
#define COLLISION_BOTTOM playerTop <= it.bottom && playerTop >= it.bottom - COLLISION_OFFSET && playerLeft <= it.right && playerRight >= it.left
#define COLLISION_LEFT playerLeft <= it.right && playerRight >= it.right
#define COLLISION_RIGHT playerRight >= it.left && playerLeft <= it.left

// left stairs
#define COLLISION_STAIR_LEFT playerLeft >= bottom.x && playerLeft <= bottom.x + 5 && playerBottom + 32 == bottom.y
#define COLLISION_STAIR_LEFT_TOP playerLeft <= top.x && playerBottom <= top.y - 32 && distanceTop < 64
#define COLLISION_STAIR_LEFT_TOP_EXIT playerLeft >= top.x && playerBottom <= top.y && distanceTop < 35 // Magic numbers TODO: could cause bugs in stairs - also check change to left!

// right stairs
#define COLLISION_STAIR_RIGHT playerRight >= bottom.x + 10 && playerBottom + 32 == bottom.y // TODO: + 10 taking player sprite size
#define COLLISION_STAIR_RIGHT_TOP playerRight >= top.x && playerBottom <= top.y - 32 && distanceTop < 64
#define COLLISION_STAIR_RIGHT_TOP_EXIT playerRight <= top.x && playerBottom <= top.y && distanceTop < 35

struct ObjectPositions {
	float top;
	float bottom;
	float left;
	float right;
	bool isAngleRight;
};

class Collision
{
private:  
	sf::RectangleShape updatePositions(std::vector<ObjectPositions> &t, ObjectPositions &m_objectPosition, sf::RectangleShape collisionBox);
	void updatePositions(ObjectPositions m_objectPositions);

	const char LEFT = 'l', RIGHT = 'r', TOP = 't', BOTTOM = 'b', NO_COLLISION = 'n', STAIR_LEFT = 'z', STAIR_RIGHT = 'x', STAIR_NONE = 'c';
	const float COLLISION_OFFSET = 15.0f; // offset for landing
	const float NO_CHANGE_GROUND_HEIGHT = 0.0f;

	float playerLeft, playerRight, playerTop, playerBottom;
	float maxStairHeight = 0;

	std::vector<ObjectPositions> staticObjectPositions;
	std::vector<ObjectPositions> staticPlatformPositions;
	std::vector<ObjectPositions> stairs;
	std::vector<ObjectPositions> entityPositions;

	sf::RectangleShape playerRectangle;
	sf::RectangleShape collisionRectangle;

public:
	Collision();
	~Collision();
	void updateObjectPosition(std::function<double(char c)> position, char t);
	void updateObjectPosition(std::function<double(char c)> position, bool isRightAngle);
	void checkCollision(std::function<void(char c, float i)> playerCollision);
	bool checkCollision();
	void checkCollisionStairs(sf::Vector2f bottom, sf::Vector2f top, std::function<void(sf::Vector2f b, sf::Vector2f t, bool onStairsBottom, bool onStairsTop, bool isStairsLeft)> playerCheckStairs, bool isLeft);
	void clearCollisionData();
};

