#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <memory>
#include "Animation.h"
#include <memory>
#include "DataRoom.h"

class Sprite {
private:


public:
	Sprite();
	Sprite(const int screenWidth, const int screenHeight, sf::Texture);
	~Sprite();
	void draw(sf::RenderWindow *window);
	virtual void update(const float dt) = 0;
	virtual void setStartPosition() = 0;
	virtual float getCollision(const char side);
	virtual bool isHarmful();
	virtual bool isGap();
	char objectId;

protected:
	std::unique_ptr<Animation> animation = nullptr;
	sf::Sprite createSprite();
	sf::IntRect createSourceRect();
	sf::IntRect m_sourceRect;
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	int m_screenHeight;
	int m_screenWidth;
	void flipSprite(bool isFlipLeft);
	int TEXTURE_SIZE_X = 32;
	int TEXTURE_SIZE_Y = 32;

};