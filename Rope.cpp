#include "Rope.h"

Rope::Rope(int screenWidth, int screenHeight, sf::Texture texture, RoomData &roomData) :
		  Sprite(screenWidth, screenHeight, texture) {
	startPositionX = roomData.ropeX;
	startPositionY = roomData.ropeY;
	topPositionY = roomData.ropeTopY;	
	setStartPosition();
	configureRope();
}

Rope::~Rope() {}

void Rope::setStartPosition() {					 
	positionX = startPositionX;
	topPositionX = startPositionX;
	positionY = startPositionY;
	createVerticies();
	createCircle();
}

void Rope::createVerticies() {
	sf::VertexArray points(sf::LinesStrip, 2);
	points[0].position = sf::Vector2f(topPositionX, topPositionY);
	points[1].position = sf::Vector2f(positionX, positionY);
	ropeVertexArray = points;
	ropeVertexArray[0].color = sf::Color::Red;
	ropeVertexArray[1].color = sf::Color::Blue;
}

// create a circle as a 'track' for the rope
void Rope::createCircle() {
	float distance = positionY - topPositionY;
	sf::CircleShape tempCircle;
	tempCircle.setRadius(distance);
	tempCircle.setPointCount(30);
	tempCircle.setPosition(sf::Vector2f(topPositionX, topPositionY));
	tempCircle.setOrigin(sf::Vector2f(tempCircle.getRadius(), tempCircle.getRadius()));
	//tempCircle.setOutlineColor(sf::Color::Red);
	//tempCircle.setFillColor(sf::Color::Transparent);
	//tempCircle.setOutlineThickness(2);
	circle = tempCircle; 	
}

void Rope::configureRope() {
	speed = (2 * PI) / rotationTime;
	angle = std::fabs(atan2(ropeVertexArray[1].position.y - ropeVertexArray[0].position.y, ropeVertexArray[1].position.x - ropeVertexArray[0].position.x));
}


float Rope::updateSpeed(float currentAngle)	{ 
	if (currentAngle * 180 / PI > 90) {
		speed -= ropeSpeed;
	}
	else if (currentAngle * 180 / PI < 90) {
		speed += ropeSpeed;
	}
	return speed;
}

void Rope::update(const float dt) {
	angle += updateSpeed(angle) * dt / 1000;
	ropeVertexArray[1].position.x = std::cos(angle) * circle.getRadius() + ropeVertexArray[0].position.x;
	ropeVertexArray[1].position.y = std::sin(angle) * circle.getRadius() + ropeVertexArray[0].position.y;
}

void Rope::draw(sf::RenderWindow * window) {
	window->draw(ropeVertexArray);
	//window->draw(circle); // TODO: Temp
}

