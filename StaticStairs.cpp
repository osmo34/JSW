#include "StaticStairs.h"


StaticStairs::StaticStairs(const int screenWidth, const int screenHeight, sf::Texture texture, RoomData &roomData) {
	sf::Vector2f bottom(roomData.stairBottomX, roomData.stairBottomY);
	sf::Vector2f top(roomData.stairTopX, roomData.stairTopY);
	createVerticies(bottom, top);
	(top.x > bottom.x) ? isLeft = false : isLeft = true;	
}

void StaticStairs::createVerticies(sf::Vector2f bottom, sf::Vector2f top) {
	sf::VertexArray testPoints(sf::LinesStrip, 2);
	testPoints[0].position = bottom;
	testPoints[1].position = top;
	va = testPoints;
	va[0].color = sf::Color::Black;
	va[1].color = sf::Color::Black;
}

StaticStairs::~StaticStairs()	{}

sf::Vector2f StaticStairs::getBottomStair() {
	return va[0].position;
}

sf::Vector2f StaticStairs::getTopStair() {
	return va[1].position;
}

bool StaticStairs::isStairsLeft() {
	return isLeft;
}

void StaticStairs::draw(sf::RenderWindow * window) {
	window->draw(va);
}

