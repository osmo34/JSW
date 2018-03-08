#pragma once
#include <SFML\Graphics.hpp>

class Animation
{
public:
	Animation();
	~Animation();
	void updateAnimation(float dt, sf::IntRect &rect, sf::Sprite *sprite);
};

