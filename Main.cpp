// Bug fixes

#include <SFML\Graphics.hpp>
#include "GameManager.h"

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
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "JSB");
	sf::Clock clock;
	window.setFramerateLimit(FPS);
	window.setVerticalSyncEnabled(true);

	std::unique_ptr <GameManager> gameManager(new GameManager(SCREEN_WIDTH, SCREEN_HEIGHT));
	gameManager->initializeGame();
	
	while (window.isOpen()) {		
		handlePollEvents(&window);
		sf::Time time = clock.getElapsedTime();
		clock.restart().asSeconds();
		gameManager->update(time.asMilliseconds());	 
		window.clear(sf::Color::Color(217, 217, 217, 255));		
		gameManager->draw(&window);		
		window.display();
	}
	return 0;
}
