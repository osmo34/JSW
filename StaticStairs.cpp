#include "StaticStairs.h"


StaticStairs::StaticStairs(bool isLeft, sf::Vector2f bottom, sf::Vector2f top) : isLeft(isLeft) {
	testVerticies(bottom, top);
}


StaticStairs::~StaticStairs()	{}

void StaticStairs::testVerticies(sf::Vector2f bottom, sf::Vector2f top) {
	sf::VertexArray testPoints(sf::LinesStrip, 2);
	testPoints[0].position = bottom;
	testPoints[1].position = top;
	va = testPoints;
}

sf::Vector2f StaticStairs::getBottomStair() {
	return va[0].position;
}

sf::Vector2f StaticStairs::getTopStair() {
	return va[1].position;
}


void StaticStairs::draw(sf::RenderWindow * window) {
	window->draw(va);
}
