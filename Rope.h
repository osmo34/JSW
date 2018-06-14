#pragma once
#include "Sprite.h"
#define PI 3.14159265359

class Rope :
	public Sprite
{
public:
	Rope(int screenWidth, int screenHeight, sf::Texture texture, RoomData &roomData);	
	~Rope();
	void update(const float dt);
	void draw(sf::RenderWindow *window);

private:
	float angle;
	float speed;
	const float rotationTime = 5.0f; // time in seconds to rotate assuming no speed loss
	const float ropeSpeed = 0.0095f; // TODO: prob wont be const long term - will be dependent on rope length!	
	void setStartPosition();
	float startPositionX;
	float startPositionY;
	float topPositionY;
	float topPositionX;
	float positionX;
	float positionY;
	sf::VertexArray ropeVertexArray;
	sf::CircleShape circle;
	void createVerticies();
	void createCircle();
	float updateSpeed(float currentAngle);
	void configureRope();
};

