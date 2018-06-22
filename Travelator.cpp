#include "Travelator.h"

Travelator::Travelator(const int screenWidth, const int screenHeight, sf::Texture texture, RoomData & roomData) :
	posX(roomData.positionX), posY(roomData.positionY),
	speed(roomData.speedX), StaticPlatform(screenWidth, screenHeight, texture, roomData) {
	setStartPosition();
}

Travelator::~Travelator() {}


float Travelator::getSpeed() {
	return speed;
}

void Travelator::setStartPosition() {
	m_sprite.setPosition(sf::Vector2f(posX, posY));
	m_sprite.setOrigin(sf::Vector2f(0, 0));
}
