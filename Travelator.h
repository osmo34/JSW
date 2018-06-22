#pragma once
#include "StaticPlatform.h"
class Travelator :
	public StaticPlatform
{
public:
	Travelator(const int screenWidth,
		const int screenHeight,
		sf::Texture texture,
		RoomData &roomData);
	~Travelator();
	float getSpeed();
	const char objectId = '£';
	

private:
	float speed;
	float posX;
	float posY;
	void setStartPosition();

};

