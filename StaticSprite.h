#pragma once
#include "Sprite.h"
class StaticSprite :
	public Sprite
{
public:
	StaticSprite(int screenWidth, int screenHeight, sf::Texture texture, RoomData &roomData);
	~StaticSprite();
	void update(const float dt);

private:
	int TEXTURE_SIZE_X;
	int TEXTURE_SIZE_Y;
	int posX;
	int posY;
	void setStartPosition();
};

