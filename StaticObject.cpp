#include "StaticObject.h"

StaticObject::StaticObject(int screenWidth,	int screenHeight, sf::Texture texture, RoomData &roomData)
	: posX(roomData.positionX), posY(roomData.positionY), 
	Sprite(screenWidth, screenHeight, texture) {	
	setStartPosition();
}							   

StaticObject::~StaticObject(){}

void StaticObject::setStartPosition() {
	m_sprite.setPosition(sf::Vector2f(posX, posY));
	m_sprite.setOrigin(sf::Vector2f(0, 0));
}

void StaticObject::update(const float dt) { }
