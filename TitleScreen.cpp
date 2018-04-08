#include "TitleScreen.h"  

TitleScreen::~TitleScreen()	{}

void TitleScreen::update(const float dt) {
	setStartPosition();
	input();
}

bool TitleScreen::getTitleScreenStatus() {
	return isOnTitleScreen;
}

void TitleScreen::setStartPosition() {
	TEXTURE_SIZE_X = m_screenWidth;
	TEXTURE_SIZE_Y = m_screenHeight;
	sf::IntRect sourceRect(0, 0, TEXTURE_SIZE_X, TEXTURE_SIZE_Y);
	m_sprite.setTextureRect(sourceRect);
	m_sprite.setPosition(sf::Vector2f(0, 0));
}

void TitleScreen::input() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
		isOnTitleScreen = false;
	}
}