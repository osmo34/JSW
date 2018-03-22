#include "StairTest.h"



StairTest::StairTest()
{
	testVerticies();
}


StairTest::~StairTest()
{
}

void StairTest::testVerticies()
{
	sf::VertexArray testPoints(sf::Points, 2);
	testPoints[0].position = sf::Vector2f(300, 702);
	testPoints[1].position = sf::Vector2f(100, 500);
	va = testPoints;
}

sf::Vector2f StairTest::getBottomStair()
{
	return va[0].position;
}

sf::Vector2f StairTest::getTopStair()
{
	return va[1].position;
}


void StairTest::draw(sf::RenderWindow * window)
{
	window->draw(va);
}
