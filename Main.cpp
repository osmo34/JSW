// Bug fixes
// TODO: - low priority - Fix bug in player/collision whereby you can stick to the bottom of platforms by holding jump
// TODO: - low priority - in player 'isAtMaxJumpHeight' is never made true. This may be causing a rare bug whereby the player flies off into the sky

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <SFML\Graphics.hpp>
#include <cstdint>
#include <fstream>
#include "Player.h"
#include "StaticObject.h"
#include "Collision.h"
#include "EnemyMoving.h"
#include "EnemyStatic.h"
#include "Pickup.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int FPS = 60;

#pragma pack(push, 2)
struct RoomData {
	std::uint8_t objectType;
	std::uint8_t textureId;
	float positionX;
	float positionY;
	float speedX;
	float speedY;
};
#pragma pack(pop)

#pragma pack(push, 2)
struct Room {
	std::uint32_t roomNumber;
	RoomData roomData[256];
};
#pragma pack(pop)

// TODO: likely use this when we have more levels
#pragma pack(push, 2)
struct World {
	Room rooms[256];
};
#pragma pack(pop)


// UPDATES
// single items and their collision - in reality this is for the player
template <typename T>
void update(const std::shared_ptr<T> &t,
	const std::shared_ptr<Collision> &collision,
	const float dt) {
	t->update(dt);
	collision->updateObjectPosition([&](char c) -> float { return t->getCollision(c); }, t->objectId);
	collision->checkCollision([&](char c, float i) { t->collision(c, i); });
}

// moving objects that kill our player - e.g. enemies
template <typename T>
void update(const std::vector<std::shared_ptr<T>> &t, const std::shared_ptr<Collision> &collision, const std::shared_ptr<Player> &player,  
	const float dt) {
	for (auto &it : t) {
		it->update(dt);
		collision->updateObjectPosition([&](char c) -> float { return it->getCollision(c); }, it->objectId); 
		if (collision->checkCollision()) {
			player->collisionEntity(it->isHarmful());
		}
	}
}

// anything else in a vector with no collision e.g. background animations - TODO: currently unused
template <typename T>
void update(const std::vector<std::shared_ptr<T>> &t, const float dt) {
	for (auto &it : t) {
		it->update(dt);
	}
}
// end updates

// DRAWING
// single objects
template <typename T>
void draw(const std::shared_ptr<T> &t, sf::RenderWindow *window) {
	t->draw(window);
}
// vectors
template <typename T>
void draw(const std::vector<std::shared_ptr<T>> &t, sf::RenderWindow *window) {
	for (auto &it : t) {
		it->draw(window);
	}
}
// ends drawing

void handlePollEvents(sf::RenderWindow *window) {
	sf::Event event;   
	while (window->pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			window->close();
			break;
		}
	}			  
}
			   
int main() {
	// TODO: enum
	const char PLAYER = 'p', STATIC_OBJECT = 's', ENEMY = 'e', ENEMY_MOVING = 'm', ENEMY_STATIC = 'n', PICK_UP = 'u';
	const char TEST_TEXTURE = 't';

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "JSW");
	sf::Clock clock;
	window.setFramerateLimit(FPS);
	window.setVerticalSyncEnabled(true);

	std::vector<std::shared_ptr<StaticObject>> levelStaticObjects;
	std::vector<std::shared_ptr<Enemy>> enemies;
	std::vector<std::shared_ptr<Pickup>> pickups;

	// load textures
	std::string playerTexture = "Ball.png";	
	sf::Texture pTexture;
	if (!pTexture.loadFromFile(playerTexture)) { std::cout << "texture load failure"; }
	// end load textures

	// create player
	std::shared_ptr <Player> player(new Player(SCREEN_WIDTH, SCREEN_HEIGHT, pTexture));



	// TODO: test code - eventually refactor elsewhere

	/*
	Room room;		  
	room.roomNumber = 0;

	// create level objects
	RoomData roomData;
	roomData.objectType = STATIC_OBJECT;
	roomData.textureId = TEST_TEXTURE;
	roomData.positionX = 600.0f;
	roomData.positionY = SCREEN_HEIGHT - 50.0f;
	roomData.speedX = 0.0f;
	roomData.speedY = 0.0f;

	RoomData roomData1;
	roomData1.objectType = STATIC_OBJECT;
	roomData1.textureId = TEST_TEXTURE;
	roomData1.positionX = 800.0f;
	roomData1.positionY = SCREEN_HEIGHT - 50.0f;
	roomData1.speedX = 0.0f;
	roomData1.speedY = 0.0f;

	RoomData roomData2;
	roomData2.objectType = STATIC_OBJECT;
	roomData2.textureId = TEST_TEXTURE;
	roomData2.positionX = 900.0f;
	roomData2.positionY = SCREEN_HEIGHT - 110.0f;
	roomData2.speedX = 0.0f;
	roomData2.speedY = 0.0f;

	RoomData roomData3;
	roomData3.objectType = STATIC_OBJECT;
	roomData3.textureId = TEST_TEXTURE;
	roomData3.positionX = 1000.0f;
	roomData3.positionY = SCREEN_HEIGHT - 170.0f;
	roomData3.speedX = 0.0f;
	roomData3.speedY = 0.0f;

	RoomData enemy1;
	enemy1.objectType = ENEMY_MOVING;
	enemy1.textureId = TEST_TEXTURE;
	enemy1.positionX = 50.0f;
	enemy1.positionY = SCREEN_HEIGHT - 50.0f;
	enemy1.speedX = 0.0f;
	enemy1.speedY = 0.1f;

	RoomData enemy2;
	enemy2.objectType = ENEMY_MOVING;
	enemy2.textureId = TEST_TEXTURE;
	enemy2.positionX = 50.0f;
	enemy2.positionY = SCREEN_HEIGHT - 50.0f;
	enemy2.speedX = 0.1f;
	enemy2.speedY = 0.0f;

	RoomData enemy3;
	enemy3.objectType = ENEMY_STATIC;
	enemy3.textureId = TEST_TEXTURE;
	enemy3.positionX = 60.0f;
	enemy3.positionY = SCREEN_HEIGHT - 50.0f;
	enemy3.speedX = 0.0f;
	enemy3.speedY = 0.0f;

	RoomData pickup;
	pickup.objectType = PICK_UP;
	pickup.textureId = TEST_TEXTURE;
	pickup.positionX = 100;
	pickup.positionY = SCREEN_HEIGHT - 55.0f;
	pickup.speedX = 0.0f;
	pickup.speedY = 0.0f; 

	// end create objects
	
	room.roomData[0] = roomData;
	room.roomData[1] = roomData1;
	room.roomData[2] = roomData2;
	room.roomData[3] = roomData3;
	room.roomData[4] = enemy1;
	room.roomData[5] = enemy2;
	room.roomData[6] = enemy3;
	room.roomData[7] = pickup;


	
	std::ofstream outputFile;
	std::string fileName = "test.jsb";

	outputFile.open(fileName, std::ios::binary);

	if (outputFile.is_open()) {
		outputFile.write(reinterpret_cast<char*>(&room), sizeof(room));
		outputFile.close();
	}
	else {
		std::cout << "write failed";
	}
	*/

	std::string fileName = "test.jsb";

	Room room{};
	std::ifstream inFile;
	inFile.open(fileName, std::ios::binary);

	if (inFile.is_open()) {
		inFile.read(reinterpret_cast<char*>(&room), sizeof(Room));
		inFile.close();
	}
	else {
		std::cout << "read failed";
	}							 

	for (int i = 0; i < 8; i++) {
		sf::Texture texture;
		switch (room.roomData[i].textureId) {
		case TEST_TEXTURE:
			texture = pTexture;
			break;
		}

		switch (room.roomData[i].objectType) {
		case STATIC_OBJECT:
		{
			std::shared_ptr <StaticObject> staticObject(new StaticObject(SCREEN_WIDTH, SCREEN_HEIGHT, texture, room.roomData[i].positionX, room.roomData[i].positionY));
			levelStaticObjects.push_back(staticObject);
		}
		break;
		case ENEMY_MOVING:
		{
			std::shared_ptr <Enemy> enemy(new EnemyMoving(SCREEN_WIDTH, SCREEN_HEIGHT, texture, room.roomData[i].positionX, room.roomData[i].positionY, room.roomData[i].speedX, room.roomData[i].speedY, ENEMY));
			enemies.push_back(enemy);
		}
		break;
		case ENEMY_STATIC:
		{
			std::shared_ptr <Enemy> enemy(new EnemyStatic(SCREEN_WIDTH, SCREEN_HEIGHT, texture, room.roomData[i].positionX, room.roomData[i].positionY, room.roomData[i].speedX, room.roomData[i].speedY, ENEMY));
			enemies.push_back(enemy);
		}
		break;
		case PICK_UP:
		{
			std::shared_ptr <Pickup> pickup(new Pickup(SCREEN_WIDTH, SCREEN_HEIGHT, texture, room.roomData[i].positionX, room.roomData[i].positionY, PICK_UP));
			pickups.push_back(pickup);
		}
		break;
		}
	}

	// TODO: end test code

	// create collision
	std::shared_ptr <Collision> collision(new Collision());

	// create static objects collision out of the main loop	as they are not going to move
	for (auto it : levelStaticObjects) {
		collision->updateObjectPosition([&](char c) -> float { return it->getCollision(c); }, it->objectId);
	}

	while (window.isOpen()) {
		handlePollEvents(&window);
		sf::Time time = clock.getElapsedTime();
		clock.restart().asSeconds();

		update(player, collision, time.asMicroseconds());
		update(enemies, collision, player, time.asMilliseconds());
		update(pickups, collision, player, time.asMilliseconds());
		
		window.clear();

		draw(player, &window);		   
		draw(levelStaticObjects, &window);
		draw(enemies, &window);
		draw(pickups, &window);

		window.display();
	}
	return 0;
}
