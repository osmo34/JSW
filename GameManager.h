#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <functional>
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
#include "LoadTextFile.h"
#include "StaticStairs.h"
#include "LevelInfo.h"
#include "MediaPlayer.h"
#include "TitleScreen.h"

struct LevelObjects {
	std::vector<std::shared_ptr<StaticObject>> levelStaticObjects;
	std::vector<std::shared_ptr<StaticPlatform>> levelStaticPlatforms;
	std::vector<std::shared_ptr<StaticStairs>> levelStaticStairs;
	std::vector<std::shared_ptr<EnemyStatic>> enemiesStatic;
	std::vector<std::shared_ptr<EnemyMoving>> enemiesMoving;
	std::vector<std::shared_ptr<Pickup>> pickups;
};

class GameManager
{
public:
	GameManager(int screenWidth, int screenHeight);
	~GameManager();
	void initializeGame();
	void update(float dt);
	void draw(sf::RenderWindow *window);

private:
	// Templates for creating objects, updates and drawing:
	
	template <typename T>
	void createObject(std::vector<std::shared_ptr<T>> &t, RoomData &room,
		const sf::Texture &texture) {
		t.push_back(std::shared_ptr<T>(new T(m_screenWidth, m_screenHeight, texture, room)));
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

	enum class Game { TITLE_SCREEN, GAME, DEAD };
	Game game = Game::TITLE_SCREEN;
	
	// private methods
	void clearRoomObjects(LevelObjects &levelObjects);
	Room createRoom(std::string fileName, LevelObjects &levelObjects, std::map<int, sf::Texture> &textures);
	void loadTexture(std::map<int, sf::Texture> &textures, std::string fileName, int id);
	void checkLevelChange();
	void checkSoundEffects();
	
	// member variables
	const std::string TEXTURES_FILE_NAME = "textures.txt";
	const std::string WORLD_FILE_NAME = "world.txt";
	int m_screenWidth = 0;
	int m_screenHeight = 0;
	LevelObjects levelObjects{};
	std::vector<std::string> worldList;
	std::vector<std::string> textureList;
	std::map<int, sf::Texture> textures;
	std::shared_ptr <LoadTextFile> loadTextures = nullptr;
	World world{};
	Room room{};
	std::shared_ptr <LoadTextFile> loadWorld = nullptr;
	std::shared_ptr <Player> player = nullptr;
	std::shared_ptr <Collision> collision = nullptr;
	std::unique_ptr <MediaPlayer> mediaPlayer = nullptr;
	std::unique_ptr <TitleScreen> titleScreen = nullptr;
	bool isFirstRun = true;
	bool inLevel = true;
	bool firstLoopComplete = false;
	int musicVolume = 10;
	int soundEffectVolume = 100;

	void changeLevel(int nextRoom);
	bool playedJumpSound = false;
	bool playedDeadSound = false;
	bool playedPickupSound = false;
};

