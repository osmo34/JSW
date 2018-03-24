#pragma once
#include <SFML\Graphics.hpp>

// TODO: created as a test, move into stair class in due course

class StairTest
{
public:
	StairTest(bool isLeft, sf::Vector2f bottom, sf::Vector2f top);
	~StairTest();
	void testVerticies(sf::Vector2f bottom, sf::Vector2f top);

	sf::Vector2f getBottomStair();
	sf::Vector2f getTopStair();

	bool isLeft;

	sf::VertexArray va;
	void draw(sf::RenderWindow *window);
};

