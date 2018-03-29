#pragma once
#include <SFML\Graphics.hpp>

// TODO: created as a test, move into stair class in due course

class StaticStairs
{
public:
	StaticStairs(sf::Vector2f bottom, sf::Vector2f top);
	~StaticStairs();
	sf::Vector2f getBottomStair();
	sf::Vector2f getTopStair();
	
	bool isStairsLeft();

	void draw(sf::RenderWindow *window);

private:
	void createVerticies(sf::Vector2f bottom, sf::Vector2f top);
	bool isLeft;
	sf::VertexArray va;
};

