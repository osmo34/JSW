#include "Animation.h"
#include <iostream>



Animation::Animation()
{
}


Animation::~Animation(){}

void Animation::update(float dt) {
	animationCalculation += dt / 10;	
}

void Animation::updateAnimation(float dt, sf::IntRect &rect, sf::Sprite *sprite, bool isFlipped) {
	// TODO: hacky

	if (animationCalculation > dt) {
		if ((rect.left >= 96 && !isFlipped) || (rect.left >= 128 && isFlipped)) {
			rect.left -= 96;
		}		
		else {
			rect.left += 32;
		}
		sprite->setTextureRect(rect);
		animationCalculation = 0.0f;		
	}
}

