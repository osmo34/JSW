#include "Animation.h"



Animation::Animation()
{
}


Animation::~Animation()
{
}

void Animation::updateAnimation(float dt, sf::IntRect &rect, sf::Sprite *sprite)
{
	if (rect.left >= 32) {
		rect.left -= 32;
	}
	else {
		rect.left += (1.0) + dt / 100;
	}
	sprite->setTextureRect(rect);
}
