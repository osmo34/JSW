#include "Sprite.h"



Sprite::Sprite() = default;

Sprite::Sprite(int screenWidth, int screenHeight, sf::Texture texture)
	: m_screenWidth(screenWidth), m_screenHeight(screenHeight), m_texture(texture) {	
	m_sprite = createSprite();

}

Sprite::~Sprite(){}

void Sprite::test(sf::Clock *clock)
{
	sf::Clock c = *clock;
	if (m_sourceRect.left >= 32) {		
		m_sourceRect.left -= 32;
	}
	else {
		m_sourceRect.left += 1.0 + c.getElapsedTime().asSeconds();
	}
	m_sprite.setTextureRect(m_sourceRect);
	
}

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

sf::Sprite Sprite::createSprite() {
	m_sourceRect = createSourceRect();
	sf::Sprite sprite(m_texture, m_sourceRect);
	sprite.setTextureRect(m_sourceRect);
	return sprite;
}

sf::IntRect Sprite::createSourceRect() {
	sf::IntRect sourceRect(0, 0, TEXTURE_SIZE_X, TEXTURE_SIZE_Y);
	return sourceRect;
}

void Sprite::draw(sf::RenderWindow * window) {
	window->draw(m_sprite);
}