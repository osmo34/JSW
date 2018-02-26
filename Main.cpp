#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <SFML\Graphics.hpp>
#include "Player.h"
#include "StaticObject.h"
#include "Collision.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int FPS = 60;

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
		
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "JSW");
	sf::Clock clock;
	window.setFramerateLimit(FPS);
	window.setVerticalSyncEnabled(true);

	std::vector<std::shared_ptr<StaticObject>> levelStaticObjects;

	// load textures
	std::string playerTexture = "Ball.png";	
	sf::Texture pTexture;
	if (!pTexture.loadFromFile(playerTexture)) { std::cout << "fail"; }
	// end load textures

	// create player
	std::unique_ptr <Player> player(new Player(SCREEN_WIDTH, SCREEN_HEIGHT, pTexture));

	// TODO: test code - static objects
	std::shared_ptr <StaticObject> testObj(new StaticObject(SCREEN_WIDTH, SCREEN_HEIGHT, pTexture, 20, 20));
	std::shared_ptr <StaticObject> testObj2(new StaticObject(SCREEN_WIDTH, SCREEN_HEIGHT, pTexture, 40, 20));
	levelStaticObjects.push_back(testObj);
	levelStaticObjects.push_back(testObj2);
	// end test code

	// create collision
	std::unique_ptr <Collision> collision(new Collision());

	// create static objects collision out of the main loop	as they are not going to move
	for (auto &it : levelStaticObjects) {
		auto collisionStatic = [&](char c) -> float { return it->getCollision(c); };
		collision->updateStaticObjectPosition(collisionStatic);
	}

	while (window.isOpen()) {
		handlePollEvents(&window);
		sf::Time time = clock.getElapsedTime();
		clock.restart().asSeconds();

		player->update(time.asMicroseconds());

		auto collisionPlayer = [&](char c) -> float { return player->getCollision(c); };
		collision->updatePlayerPosition(collisionPlayer);

		window.clear();
		player->draw(&window);
		
		for (auto &it : levelStaticObjects) {
			it->draw(&window);
		}

		window.display();
	}

	return 0;
}