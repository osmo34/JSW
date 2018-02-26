#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <memory>

class Sprite {
private:

public:
	Sprite();
	Sprite(const Sprite &other);
	Sprite(const int screenWidth, const int screenHeight, sf::Texture);
	~Sprite();
	void draw(sf::RenderWindow *window);
	virtual void update(const float dt) = 0;
	virtual void setStartPosition() = 0;
	virtual float getCollision(const char side);

protected:
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	int m_screenHeight;
	int m_screenWidth;	
};