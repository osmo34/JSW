// Bug fixes
// TODO: - low priority - Fix bug in player/collision whereby you can stick to the bottom of platforms by holding jump
// TODO: - low priority - in player 'isAtMaxJumpHeight' is never made true. This may be causing a rare bug whereby the player flies off into the sky

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

	std::shared_ptr <GameManager> gameManager(new GameManager(SCREEN_WIDTH, SCREEN_HEIGHT));
	gameManager->initializeGame();
	
	while (window.isOpen()) {		
		handlePollEvents(&window);
		sf::Time time = clock.getElapsedTime();
		clock.restart().asSeconds();
		gameManager->update(time.asMilliseconds());
	 
		window.clear();
		gameManager->draw(&window);		
		window.display();
	}
	return 0;
}
