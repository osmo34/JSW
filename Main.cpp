// Bug fixes
// TODO: - low priority - Fix bug in player/collision whereby you can stick to the bottom of platforms by holding jump
// TODO: - low priority - in player 'isAtMaxJumpHeight' is never made true. This may be causing a rare bug whereby the player flies off into the sky

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <SFML\Graphics.hpp>
#include "Player.h"
#include "StaticObject.h"
#include "StaticPlatform.h"
#include "Collision.h"
#include "EnemyMoving.h"
#include "EnemyStatic.h"
#include "Pickup.h"
#include "DataRoom.h"
#include "LoadRoom.h"
#include "WriteRoom.h"
#include "LoadTextures.h"
#include "StaticStairs.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int FPS = 60;

// create object in their vectors
template <typename T>
void createObject(std::vector<std::shared_ptr<T>> &t, RoomData &room,
	const sf::Texture &texture) { 	
	t.push_back(std::shared_ptr<T>(new T(SCREEN_WIDTH, SCREEN_HEIGHT, texture, room)));	  // room.positionX, room.positionY, objectId, room.speedX, room.speedY
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

// specific for stairs, no requirement for it to be a template at this time
void update(const std::vector<std::shared_ptr<StaticStairs>> &stairs, std::shared_ptr<Collision> &collision, std::shared_ptr<Player> &player) {
	for (auto it : stairs) {
		collision->checkCollisionStairs(it->getBottomStair(), it->getTopStair(),
		([&](sf::Vector2f b, sf::Vector2f t, bool c, bool d, bool e) { player->onStairs(b, t, c, d, e); }), it->isStairsLeft());
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

Room createRoom(std::vector<std::shared_ptr<StaticObject>> &levelStaticObjects,
				std::vector<std::shared_ptr<StaticPlatform>> &levelStaticPlatforms,
				std::vector<std::shared_ptr<StaticStairs>> &levelStaticStairs,
				std::vector<std::shared_ptr<EnemyStatic>> &enemiesStatic,
				std::vector<std::shared_ptr<EnemyMoving>> &enemiesMoving,
				std::vector<std::shared_ptr<Pickup>> &pickups,				
				std::map<int, sf::Texture> &textures) {
	const char PLAYER = 'p', STATIC_OBJECT = 's', ENEMY = 'e', STATIC_PLATFORM = 't', STATIC_STAIRS = 'l', ENEMY_MOVING = 'm', ENEMY_STATIC = 'n', PICK_UP = 'u';
	std::string fileName = "test.jsb";
	Room room{};
	
	std::shared_ptr<LoadRoom> loadLevel(new LoadRoom);
	room = loadLevel->loadRoom(fileName);

	for (int i = 0; i < room.numObjects; i++) {
		sf::Texture texture;
		texture = textures.find(room.roomData[i].textureId)->second;

		switch (room.roomData[i].objectType) {
		case STATIC_OBJECT:
			createObject(levelStaticObjects, room.roomData[i], texture);
			break;
		case STATIC_PLATFORM:
			createObject(levelStaticPlatforms, room.roomData[i], texture);
			break;
		case STATIC_STAIRS:
			createObject(levelStaticStairs, room.roomData[i], texture);
			break;
		case ENEMY_MOVING:
			createObject(enemiesMoving, room.roomData[i], texture);
			break;
		case ENEMY_STATIC:
			createObject(enemiesStatic, room.roomData[i], texture);
			break;
		case PICK_UP:
			createObject(pickups, room.roomData[i], texture);
			break;
		default:
			std::cout << "error in room data";
		}
	}
	return room; 		
}

void loadTexture(std::map<int, sf::Texture> &textures, std::string fileName, int id) {
	sf::Texture texture;
	if (!texture.loadFromFile(fileName)){ std::cout << "texture load failure - " << fileName ; }
	textures[id] = texture;	
}
			   
int main() {
	std::string fileName = "test.jsb";
	std::string texturesFileName = "textures.txt";

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "JSW");
	sf::Clock clock;
	window.setFramerateLimit(FPS);
	window.setVerticalSyncEnabled(true);

	std::vector<std::shared_ptr<StaticObject>> levelStaticObjects;
	std::vector<std::shared_ptr<StaticPlatform>> levelStaticPlatforms;
	std::vector<std::shared_ptr<StaticStairs>> levelStaticStairs;
	std::vector<std::shared_ptr<EnemyStatic>> enemiesStatic;	
	std::vector<std::shared_ptr<EnemyMoving>> enemiesMoving;
	std::vector<std::shared_ptr<Pickup>> pickups;
	std::vector<std::string> textureList;
	std::map<int, sf::Texture> textures;

	// load textures
	std::shared_ptr <LoadTextures> loadTextures(new LoadTextures(texturesFileName));
	loadTextures->loadTextures(textureList);

	for (int i = 0; i < textureList.size(); i++) {
		loadTexture(textures, textureList[i], i);		
	}
	// end load textures

	// create player
	std::shared_ptr <Player> player(new Player(SCREEN_WIDTH, SCREEN_HEIGHT, textures[0]));

	// TODO: Only call this if we want to compile. It is only here for testing
	std::shared_ptr <WriteRoom> writeRoom(new WriteRoom);
	writeRoom->createRoom(fileName, SCREEN_WIDTH, SCREEN_HEIGHT);

	// create room
	Room room{};
	room = createRoom(levelStaticObjects, levelStaticPlatforms, levelStaticStairs, enemiesStatic, enemiesMoving, pickups, textures);

	// create collision
	std::shared_ptr <Collision> collision(new Collision());

	// create static objects collision out of the main loop	as they are not going to move
	update(levelStaticObjects, collision);
	update(levelStaticPlatforms, collision);
	
	while (window.isOpen()) {
		handlePollEvents(&window);
		sf::Time time = clock.getElapsedTime();
		clock.restart().asSeconds();

		update(player, collision, time.asMilliseconds());
		update(levelStaticStairs, collision, player);
		update(enemiesMoving, collision, player, time.asMilliseconds());
		update(enemiesStatic, collision, player, time.asMilliseconds());		
		update(pickups, collision, player, time.asMilliseconds());
		window.clear();	
	
		draw(levelStaticObjects, &window);
		draw(levelStaticPlatforms, &window);
		draw(levelStaticStairs, &window);
		draw(enemiesMoving, &window);
		draw(enemiesStatic, &window);		
		draw(pickups, &window);
		draw(player, &window);
		window.display();
	}
	return 0;
}
