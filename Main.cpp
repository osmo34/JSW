// Bug fixes
// TODO: - low priority - Fix bug in player/collision whereby you can stick to the bottom of platforms by holding jump
// TODO: - low priority - in player 'isAtMaxJumpHeight' is never made true. This may be causing a rare bug whereby the player flies off into the sky

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <SFML\Graphics.hpp>
#include <cstdint>
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
struct LevelData {
	std::uint8_t objectType;
	std::uint8_t textureId;
	float positionX;
	float positionY;
};
#pragma pack(pop)

#pragma pack(push, 2)
struct Level {
	std::uint32_t levelNumber;
	LevelData levelData[256];
};
#pragma pack(pop)

// TODO: likely use this when we have more levels
#pragma pack(push, 2)
struct World {
	Level levels[256];
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
	const char PLAYER = 'p', STATIC_OBJECT = 's', ENEMY = 'e', PICK_UP = 'u';
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

	Level level;
	level.levelNumber = 0;

	// create level objects
	LevelData levelData;
	levelData.objectType = STATIC_OBJECT;
	levelData.textureId = TEST_TEXTURE;
	levelData.positionX = 600.0f;
	levelData.positionY = SCREEN_HEIGHT - 50.0f;

	LevelData levelData1;
	levelData1.objectType = STATIC_OBJECT;
	levelData1.textureId = TEST_TEXTURE;
	levelData1.positionX = 800.0f;
	levelData1.positionY = SCREEN_HEIGHT - 50.0f;

	// end create objects
	
	level.levelData[0] = levelData;
	level.levelData[1] = levelData1;

	for (int i = 0; i < 2; i++) {
		sf::Texture texture;
		switch (level.levelData[i].textureId) {
		case TEST_TEXTURE:
			texture = pTexture;
			break;
		}
		switch (level.levelData[i].objectType) {
		case STATIC_OBJECT:
			std::shared_ptr <StaticObject> testObj(new StaticObject(SCREEN_WIDTH, SCREEN_HEIGHT, texture, level.levelData[i].positionX, level.levelData[i].positionY));
			levelStaticObjects.push_back(testObj);
			break;
		}
	}





	// TODO: test code - static objects
	//std::shared_ptr <StaticObject> testObj(new StaticObject(SCREEN_WIDTH, SCREEN_HEIGHT, pTexture, 600, SCREEN_HEIGHT - 50.0f));
	//std::shared_ptr <StaticObject> testObj2(new StaticObject(SCREEN_WIDTH, SCREEN_HEIGHT, pTexture, 800, SCREEN_HEIGHT - 50.0f));
	//std::shared_ptr <StaticObject> testObj3(new StaticObject(SCREEN_WIDTH, SCREEN_HEIGHT, pTexture, 900, SCREEN_HEIGHT - 110.0f));
	//std::shared_ptr <StaticObject> testObj4(new StaticObject(SCREEN_WIDTH, SCREEN_HEIGHT, pTexture, 1000, SCREEN_HEIGHT - 170.0f));
	//levelStaticObjects.push_back(testObj);
	//levelStaticObjects.push_back(testObj2);
	//levelStaticObjects.push_back(testObj3);
	//levelStaticObjects.push_back(testObj4);

	// TODO: test code - enemies
	std::shared_ptr <Enemy> testEnemy(new EnemyMoving(SCREEN_WIDTH, SCREEN_HEIGHT, pTexture, 50, SCREEN_HEIGHT - 50.0f, 0.0f, 0.1f, ENEMY));
	std::shared_ptr <Enemy> testEnemy2(new EnemyMoving(SCREEN_WIDTH, SCREEN_HEIGHT, pTexture, 50, SCREEN_HEIGHT - 50.0f, 0.1f, 0.0f, ENEMY));
	enemies.push_back(testEnemy);
	enemies.push_back(testEnemy2);

	// TODO: test code - static enemies
	std::shared_ptr <Enemy> testEnemy3(new EnemyStatic(SCREEN_WIDTH, SCREEN_HEIGHT, pTexture, 60, SCREEN_HEIGHT - 50.0f, 0.0f, 0.0f, ENEMY));
	enemies.push_back(testEnemy3);

	// TODO: test code - pickups
	std::shared_ptr <Pickup> testPickup(new Pickup(SCREEN_WIDTH, SCREEN_HEIGHT, pTexture, 100, SCREEN_HEIGHT - 55.0f, PICK_UP));
	pickups.push_back(testPickup);
	// end test code //

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