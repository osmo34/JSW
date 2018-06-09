#include "Rope.h"

Rope::Rope(int screenWidth, int screenHeight, sf::Texture texture, RoomData &roomData) :
		  Sprite(screenWidth, screenHeight, texture) {
	startPositionX = roomData.ropeX;
	startPositionY = roomData.ropeY;
	topPositionY = roomData.ropeTopY;
	setStartPosition();
}			

Rope::~Rope() {}

void Rope::setStartPosition() {					 
	positionX = startPositionX;
	topPositionX = startPositionX;
	positionY = startPositionY;
	createVerticies();
}

void Rope::createVerticies() {
	sf::VertexArray points(sf::LinesStrip, 2);
	points[0].position = sf::Vector2f(topPositionX, topPositionY);
	points[1].position = sf::Vector2f(positionX, positionY);
	ropeVertexArray = points;
	ropeVertexArray[0].color = sf::Color::Red;
	ropeVertexArray[1].color = sf::Color::Red;
}

void Rope::update(const float dt) {

}

void Rope::draw(sf::RenderWindow * window)
{
	window->draw(ropeVertexArray);
}

