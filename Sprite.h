#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <memory>

class Sprite {
private:

public:
	Sprite();
	Sprite(int screenWidth, int screenHeight, std::string textureName);
	~Sprite();
	void draw(sf::RenderWindow *window);
	virtual void update(float dt) = 0;
	virtual void setStartPosition() = 0;

protected:
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	int m_screenHeight;
	int m_screenWidth;	
};