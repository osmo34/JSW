#include "Pickup.h"


Pickup::Pickup(const int screenWidth, 
	const int screenHeight, 
	sf::Texture texture, 
	RoomData &roomData) : posX(roomData.positionX), posY(roomData.positionY), objectId(roomData.objectType),
	Sprite(screenWidth, screenHeight, texture) {
	setStartPosition();
}

Pickup::~Pickup() {}

void Pickup::setStartPosition() {
	m_sprite.setPosition(sf::Vector2f(posX, posY));
	m_sprite.setOrigin(sf::Vector2f(0, 0));
}

void Pickup::update(const float dt)	{}

