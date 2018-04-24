#include "StaticSprite.h"

StaticSprite::StaticSprite(int screenWidth, int screenHeight, sf::Texture texture, 
	RoomData &roomData) : 
	Sprite(screenWidth, screenHeight, texture) {												   
	this->TEXTURE_SIZE_X = texture.getSize().x;
	this->TEXTURE_SIZE_Y = texture.getSize().y;
	this->posX = roomData.positionX;
	this->posY = roomData.positionY;

	setStartPosition();
}

StaticSprite::~StaticSprite(){}

void StaticSprite::setStartPosition() {
	sf::IntRect sourceRect(0, 0, TEXTURE_SIZE_X, TEXTURE_SIZE_Y);
	m_sprite.setTextureRect(sourceRect);
	m_sprite.setPosition(posX, posY);
}

void StaticSprite::update(const float dt) {}


