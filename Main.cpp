// Bug fixes
// TODO: - low priority - Fix bug in player/collision whereby you can stick to the bottom of platforms by holding jump
// TODO: - low priority - in player 'isAtMaxJumpHeight' is never made true. This may be causing a rare bug whereby the player flies off into the sky

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <SFML\Graphics.hpp>
#include "Player.h"
#include "StaticObject.h"
#include "Collision.h"
#include "EnemyMoving.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int FPS = 60;

// UPDATES
// single items and their collision - e.g. player
template <typename T>
void update(const std::shared_ptr<T> &t,
	const std::shared_ptr<Collision> &collision,
	const float dt) {
	t->update(dt);
	collision->updateObjectPosition([&](char c) -> float { return t->getCollision(c); }, t->objectId);
	collision->checkCollision([&](char c, float i) { t->collision(c, i); });
}

// moving objects in vector with collision e.g. enemy
// TODO: Untested
template <typename T>
void update(const std::vector<std::shared_ptr<T>> &t, 
	const std::shared_ptr<Collision> &collision, 
	const float dt) {
	for (auto &it : t) {
		it->update(dt);
		// TODO: update once collision is implemented
		collision->updateObjectPosition([&](char c) -> float { return it->getCollision(c); }, it->objectId); 
		//collision->checkCollision([&](char c, float i) { it->collision(c, i); }); // pass in T
	}
}

// anything else in a vector with no collision - might not get used long term
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
	const char PLAYER = 'p', STATIC_OBJECT = 's', ENEMY_MOVING = 'e';

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "JSW");
	sf::Clock clock;
	window.setFramerateLimit(FPS);
	window.setVerticalSyncEnabled(true);

	std::vector<std::shared_ptr<StaticObject>> levelStaticObjects;
	std::vector<std::shared_ptr<Enemy>> enemies;

	// load textures
	std::string playerTexture = "Ball.png";	
	sf::Texture pTexture;
	if (!pTexture.loadFromFile(playerTexture)) { std::cout << "texture load failure"; }
	// end load textures

	// create player
	std::shared_ptr <Player> player(new Player(SCREEN_WIDTH, SCREEN_HEIGHT, pTexture));

	// TODO: test code - static objects
	std::shared_ptr <StaticObject> testObj(new StaticObject(SCREEN_WIDTH, SCREEN_HEIGHT, pTexture, 600, SCREEN_HEIGHT - 50.0f));
	std::shared_ptr <StaticObject> testObj2(new StaticObject(SCREEN_WIDTH, SCREEN_HEIGHT, pTexture, 800, SCREEN_HEIGHT - 50.0f));
	std::shared_ptr <StaticObject> testObj3(new StaticObject(SCREEN_WIDTH, SCREEN_HEIGHT, pTexture, 900, SCREEN_HEIGHT - 110.0f));
	std::shared_ptr <StaticObject> testObj4(new StaticObject(SCREEN_WIDTH, SCREEN_HEIGHT, pTexture, 1000, SCREEN_HEIGHT - 170.0f));
	levelStaticObjects.push_back(testObj);
	levelStaticObjects.push_back(testObj2);
	levelStaticObjects.push_back(testObj3);
	levelStaticObjects.push_back(testObj4);

	// TODO: test code - enemies
	std::shared_ptr <Enemy> testEnemy(new EnemyMoving(SCREEN_WIDTH, SCREEN_HEIGHT, pTexture, 50, SCREEN_HEIGHT - 50.0f, 0.0f, 0.1f, ENEMY_MOVING));
	std::shared_ptr <Enemy> testEnemy2(new EnemyMoving(SCREEN_WIDTH, SCREEN_HEIGHT, pTexture, 50, SCREEN_HEIGHT - 50.0f, 0.1f, 0.0f, ENEMY_MOVING));
	enemies.push_back(testEnemy);
	enemies.push_back(testEnemy2);
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
		update(enemies, collision, time.asMilliseconds());

		window.clear();
		draw(player, &window);		   
		draw(levelStaticObjects, &window);
		draw(enemies, &window);	

		window.display();
	}

	return 0;
}