#pragma once
#include "Sprite.h"
class Rope :
	public Sprite
{
public:
	Rope(int screenWidth, int screenHeight, sf::Texture texture, RoomData &roomData);
	~Rope();
	void update(const float dt);
	void draw(sf::RenderWindow *window);

private:
	void setStartPosition();
	float startPositionX;
	float startPositionY;
	float topPositionY;
	float topPositionX;
	float positionX;
	float positionY;
	sf::VertexArray ropeVertexArray;
	void createVerticies();
};

