#pragma once
#include "Sprite.h"

// for use with solid structures which cannot be passed - e.g. a wall
class StaticObject : public Sprite
{
private:
	float posX;
	float posY;

public:
	StaticObject(const int screenWidth,
		const int screenHeight,
		sf::Texture texture,
		const float posX,
		const float posY,
		const char, const float, const float
		);
	~StaticObject();

	void update(const float dt);	
	const char objectId = 's';

protected:
	void setStartPosition();
};

