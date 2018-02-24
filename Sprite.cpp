#include "Sprite.h"

Sprite::Sprite() = default;

Sprite::Sprite(int screenWidth, int screenHeight, std::string textureName) : m_screenWidth(screenWidth), m_screenHeight(screenHeight)
{
	if (!m_texture.loadFromFile(textureName)) {
		std::cout << textureName + " texture load failed" << std::endl;
	}
	m_sprite.setTexture(m_texture);
}

Sprite::~Sprite(){}

void Sprite::draw(sf::RenderWindow * window)
{
	window->draw(m_sprite);
}
