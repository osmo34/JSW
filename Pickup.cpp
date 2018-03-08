#include "Pickup.h"

Pickup::Pickup(const int screenWidth, 
	const int screenHeight, 
	sf::Texture texture, 
	const float posX, const float posY, 
	const char objectId, float, float) : posX(posX), posY(posY), objectId(objectId),
	Sprite(screenWidth, screenHeight, texture) {
	setStartPosition();
}

Pickup::~Pickup() {}

void Pickup::setStartPosition() {
	m_sprite.setPosition(sf::Vector2f(posX, posY));
	m_sprite.setOrigin(sf::Vector2f(0, 0));
}

void Pickup::update(const float dt)	{}
