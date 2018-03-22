#pragma once
#include <SFML\Graphics.hpp>

// TODO: created as a test, move into stair class in due course

class StairTest
{
public:
	StairTest();
	~StairTest();
	void testVerticies();

	sf::Vector2f getBottomStair();
	sf::Vector2f getTopStair();


	sf::VertexArray va;
	void draw(sf::RenderWindow *window);
};

