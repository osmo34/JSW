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
#include "StaticSprite.h"
#include "LevelInfo.h"
#include "MediaPlayer.h"
#include "TitleScreen.h"

// struct to hold all game objects in one place
struct LevelObjects {
	std::vector<std::shared_ptr<StaticObject>> levelStaticObjects;
	std::vector<std::shared_ptr<StaticPlatform>> levelStaticPlatforms;
	std::vector<std::shared_ptr<StaticStairs>> levelStaticStairs;
	std::vector<std::shared_ptr<EnemyStatic>> enemiesStatic;
	std::vector <std::shared_ptr<StaticSprite>> spritesStatic;
	std::vector<std::shared_ptr<EnemyMoving>> enemiesMoving;
	std::vector<std::shared_ptr<Pickup>> pickups;
	std::shared_ptr<Player> player;
	std::shared_ptr<Collision> collision;
	std::shared_ptr<TitleScreen> titleScreen;
	std::map<int, sf::Texture> textures;
	Room room;
	World world;
};

// Game state
enum class Game { TITLE_SCREEN, GAME, DEAD };

// Game Updates. Responsible for updating all objects & loading rooms when levels change
class GameUpdates {
					 
public:
	GameUpdates(int screenWidth, int screenHeight);
	~GameUpdates();
	void updateGame(float dt, LevelObjects &levelObjects, Game &game);
private:

	// Templates for creating objects, updates and drawing:
	template <typename T>
	void createObject(std::vector<std::shared_ptr<T>> &t, RoomData &room,
		const sf::Texture &texture) {
		t.push_back(std::shared_ptr<T>(new T(screenWidth, screenHeight, texture, room)));
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

	// anything else in a vector with no collision e.g. background animations
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

	LevelObjects levelObjects{};
	Room createRoom(std::string fileName, LevelObjects &levelObjects);
	void checkLevelChange(LevelObjects &levelObjects);
	void changeLevel(int nextRoom, LevelObjects &levelObjects);
	void clearRoomObjects(LevelObjects &levelObjects);
	bool isFirstRun;
	bool inLevel; // TODO: ?? Check this
	bool firstLoopComplete;
	int screenWidth;
	int screenHeight;
};

// responsible for sound & music
class GameSounds {
public:
	GameSounds(int musicVolume, int soundEffectVolume);
	~GameSounds();
	void playMusic(std::string song);
	void checkPlayerSoundEffects(LevelObjects &levelObjects);

private:
	std::unique_ptr <MediaPlayer> mediaPlayer = nullptr;
	void initializeMediaPlayer();
	int musicVolume = 10;
	int soundEffectVolume = 100;
	bool playedJumpSound = false;
	bool playedDeadSound = false;
	bool playedPickupSound = false;
};

// responsible for drawing
class GameDraw {
public:
	GameDraw();
	~GameDraw();
	void draw(sf::RenderWindow *window, LevelObjects &levelObjects, Game &game);
private:
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
};

// Responsible for initiliazation, managing updates & drawing.
class GameManager
{
public:
	GameManager(int screenWidth, int screenHeight);
	~GameManager();
	void initializeGame();
	void update(float dt);
	void draw(sf::RenderWindow *window);

private:	
	// set game state
	Game game = Game::TITLE_SCREEN;

	// private methods	
	void loadTexture(std::map<int, sf::Texture> &textures, std::string fileName, int id); 
	
	// member variables
	const std::string TEXTURES_FILE_NAME = "textures.txt";
	const std::string WORLD_FILE_NAME = "world.txt";
	int m_screenWidth = 0;
	int m_screenHeight = 0;
	LevelObjects levelObjects{};
	std::vector<std::string> worldList;
	std::vector<std::string> textureList;
	std::shared_ptr <LoadTextFile> loadTextures = nullptr;
	std::shared_ptr <LoadTextFile> loadWorld = nullptr;
	std::unique_ptr <MediaPlayer> mediaPlayer = nullptr;
	std::unique_ptr <GameUpdates> gameUpdates = nullptr;
	std::unique_ptr <GameSounds> gameSounds = nullptr;
	std::unique_ptr <GameDraw> gameDraw = nullptr;
	bool isFirstRun = true;
	bool inLevel = true;
	bool firstLoopComplete = false;
	int musicVolume = 10;
	int soundEffectVolume = 100;
};

