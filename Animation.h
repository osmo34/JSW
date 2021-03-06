#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>

class Animation
{
public:
	Animation();
	~Animation();
	void updateAnimation(float dt, sf::IntRect &rect, sf::Sprite *sprite, bool isFlipped);
	void update(float dt);

private:
	float animationCalculation = 0.0f;
	int leftPosition = 0;	
};

