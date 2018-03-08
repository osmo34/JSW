#include "Sprite.h"



Sprite::Sprite() = default;

Sprite::Sprite(int screenWidth, int screenHeight, sf::Texture texture)
	: m_screenWidth(screenWidth), m_screenHeight(screenHeight), m_texture(texture) {	
	m_sprite = createSprite();
	animation = std::unique_ptr<Animation>(new Animation());
}

Sprite::~Sprite(){}

float Sprite::getCollision(char side) {
	const char LEFT = 'l', RIGHT = 'r', TOP = 't', BOTTOM = 'b';
	switch (side) {
	case LEFT:
		return m_sprite.getGlobalBounds().left;
		break;
	case RIGHT:
		return m_sprite.getGlobalBounds().left + m_sprite.getGlobalBounds().width;
		break;
	case TOP:
		return m_sprite.getGlobalBounds().top - m_sprite.getGlobalBounds().height;
		break;
	case BOTTOM:
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

void Sprite::flipSprite(bool isflipLeft) {
	(isflipLeft) ? 
		m_sprite.setTextureRect(sf::IntRect(m_sprite.getGlobalBounds().width, 0, 
			m_sprite.getGlobalBounds().width * -1, m_sprite.getGlobalBounds().height)) : 
		m_sprite.setTextureRect(sf::IntRect(m_sprite.getGlobalBounds().width, 0, 
			m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height));	
}

void Sprite::draw(sf::RenderWindow * window) {
	window->draw(m_sprite);
}