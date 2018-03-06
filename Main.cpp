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
#include "DataRoom.h"
#include "LoadRoom.h"
#include "WriteRoom.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int FPS = 60;

// create object in their vectors
template <typename T>
void createObject(std::vector<std::shared_ptr<T>> &t, const RoomData &room,    
	const sf::Texture &texture, const char objectId) {
	t.push_back(std::shared_ptr<T>(new T(SCREEN_WIDTH, SCREEN_HEIGHT, texture, room.positionX, room.positionY, objectId, room.speedX, room.speedY)));
}

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

// updates collision for anything out of the game loop - e.g. static objects
template <typename T>
void update(const std::vector<std::shared_ptr<T>> &t, std::shared_ptr<Collision> &collision) {
	for (auto it : t) {
		collision->updateObjectPosition([&](char c) -> float { return it->getCollision(c); }, it->objectId);
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
	const char PLAYER = 'p', STATIC_OBJECT = 's', ENEMY = 'e', ENEMY_MOVING = 'm', ENEMY_STATIC = 'n', PICK_UP = 'u';
	const char TEST_TEXTURE = 't';

	std::string fileName = "test.jsb";

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "JSW");
	sf::Clock clock;
	window.setFramerateLimit(FPS);
	window.setVerticalSyncEnabled(true);

	std::vector<std::shared_ptr<StaticObject>> levelStaticObjects;
	std::vector<std::shared_ptr<EnemyStatic>> enemiesStatic;
	std::vector<std::shared_ptr<EnemyMoving>> enemiesMoving;
	std::vector<std::shared_ptr<Pickup>> pickups;

	// load textures
	std::string playerTexture = "Ball.png";	
	sf::Texture pTexture;
	if (!pTexture.loadFromFile(playerTexture)) { std::cout << "texture load failure"; }
	// end load textures

	// create player
	std::shared_ptr <Player> player(new Player(SCREEN_WIDTH, SCREEN_HEIGHT, pTexture));

	// load the room from file
	Room room{};
	std::shared_ptr<LoadRoom> loadLevel(new LoadRoom);
	room = loadLevel->loadRoom(fileName);

	for (int i = 0; i < room.numObjects; i++) {
		sf::Texture texture;
		switch (room.roomData[i].textureId) {
		case TEST_TEXTURE:
			texture = pTexture;
			break;
		}

		switch (room.roomData[i].objectType) {
		case STATIC_OBJECT:
		{				
			createObject(levelStaticObjects, room.roomData[i], texture, NULL);
		}
		break;
		case ENEMY_MOVING:
		{				
			createObject(enemiesMoving, room.roomData[i], texture, ENEMY);
		}
		break;
		case ENEMY_STATIC:
		{	
			createObject(enemiesStatic, room.roomData[i], texture, ENEMY);
		}
		break;
		case PICK_UP:
		{				
			createObject(pickups, room.roomData[i], texture, PICK_UP);
		}
		break;
		}
	}

	// create collision
	std::shared_ptr <Collision> collision(new Collision());

	// create static objects collision out of the main loop	as they are not going to move
	update(levelStaticObjects, collision);

	while (window.isOpen()) {
		handlePollEvents(&window);
		sf::Time time = clock.getElapsedTime();
		clock.restart().asSeconds();

		update(player, collision, time.asMicroseconds());
		update(enemiesMoving, collision, player, time.asMilliseconds());
		update(enemiesStatic, collision, player, time.asMilliseconds());
		update(pickups, collision, player, time.asMilliseconds());
		
		window.clear();

		draw(player, &window);		   
		draw(levelStaticObjects, &window);
		draw(enemiesMoving, &window);
		draw(enemiesStatic, &window);
		draw(pickups, &window);

		window.display();
	}
	return 0;
}
