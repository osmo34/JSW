#include "Animation.h"



Animation::Animation()
{
}


Animation::~Animation(){}

void Animation::update(float dt) {
	animationCalculation += dt / 10;
}

void Animation::updateAnimation(float dt, sf::IntRect &rect, sf::Sprite *sprite) {
	// TODO: hacky
	if (animationCalculation > dt) {
		if (rect.left == 96) {
			rect.left -= 32;
		}
		else {
			rect.left += 32;
		}
		animationCalculation = 0.0f;
	} 
}

