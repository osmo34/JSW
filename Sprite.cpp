#include "Sprite.h"

Sprite::Sprite() = default;

Sprite::Sprite(const Sprite & other) { *this = other; }

Sprite::Sprite(int screenWidth, int screenHeight, sf::Texture texture)
	: m_screenWidth(screenWidth), m_screenHeight(screenHeight), m_texture(texture) {
	m_sprite.setTexture(m_texture);
}

Sprite::~Sprite(){}

float Sprite::getCollision(char side) {
	switch (side) {
	case 'l':
		return m_sprite.getGlobalBounds().left;
		break;
	case 'r':
		return m_sprite.getGlobalBounds().left + m_sprite.getGlobalBounds().width;
		break;
	case 't':
		return m_sprite.getGlobalBounds().top - m_sprite.getGlobalBounds().height;
		break;
	case 'b':
		return m_sprite.getGlobalBounds().top;
		break;
	default:
		return 0.0f;
	}
}

bool Sprite::isHarmful() {
	return false;
}

void Sprite::draw(sf::RenderWindow * window) {
	window->draw(m_sprite);
}