#include "StairTest.h"


StairTest::StairTest(bool isLeft, sf::Vector2f bottom, sf::Vector2f top) : isLeft(isLeft) {
	testVerticies(bottom, top);
}


StairTest::~StairTest()	{}

void StairTest::testVerticies(sf::Vector2f bottom, sf::Vector2f top) {
	sf::VertexArray testPoints(sf::LinesStrip, 2);
	testPoints[0].position = bottom;
	testPoints[1].position = top;
	va = testPoints;
}

sf::Vector2f StairTest::getBottomStair() {
	return va[0].position;
}

sf::Vector2f StairTest::getTopStair() {
	return va[1].position;
}


void StairTest::draw(sf::RenderWindow * window) {
	window->draw(va);
}
