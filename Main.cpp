#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <SFML\Graphics.hpp>
#include "Player.h"
#include "StaticObject.h"
#include <functional>

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

// TODO: test function
void getCollision(std::function<double(char c)> collision) {

	const char LEFT = 'l';
	const char RIGHT = 'r';
	const char TOP = 't';
	const char BOTTOM = 'b';

	auto l = collision(LEFT);
	auto r = collision(RIGHT);
	auto t = collision(TOP);
	auto b = collision(BOTTOM);
	std::cout << "left: " << l << std::endl;
	std::cout << "right: " << r << std::endl;
	std::cout << "top: " << t << std::endl;
	std::cout << "bottom: " << b << std::endl;
}
// end test function


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

	


	while (window.isOpen()) {
		handlePollEvents(&window);
		sf::Time time = clock.getElapsedTime();
		clock.restart().asSeconds();

		player->update(time.asMicroseconds());

		// TODO: testing lambda's for getting collision data prior to moving it into a collision class
		auto collisionPlayer = [&](char c) -> float { return player->getCollision(c); };
		getCollision(collisionPlayer);

		for (auto &it : levelStaticObjects) {
			auto collisionWall = [&](char c) -> float { return it->getCollision(c); };
			getCollision(collisionWall);
		}
		// end test code
		
		window.clear();
		player->draw(&window);
		
		for (auto &it : levelStaticObjects) {
			it->draw(&window);
		}

		window.display();
	}

	return 0;
}