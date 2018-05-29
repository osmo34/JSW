#include "GameManager.h"

GameUpdates::GameUpdates(int screenWidth, int screenHeight) : screenWidth(screenWidth), screenHeight(screenHeight) {
	isFirstRun = true;
	inLevel = true;
	firstLoopComplete = false;	
}

GameUpdates::~GameUpdates() {}

Room GameUpdates::createRoom(std::string fileName, LevelObjects & levelObjects) {
	const char PLAYER = 'p', STATIC_OBJECT = 's', ENEMY = 'e', STATIC_PLATFORM = 't', STATIC_STAIRS = 'l', STATIC_SPRITE = '#', ENEMY_MOVING = 'm', ENEMY_STATIC = 'n', PICK_UP = 'u';
	Room room{};

	std::unique_ptr<LoadRoom> loadLevel(new LoadRoom);
	room = loadLevel->loadRoom(fileName);

	for (int i = 0; i < room.numObjects; i++) {	 //  room.numObjects
		sf::Texture texture;
		texture = levelObjects.textures.find(room.roomData[i].textureId)->second;		

		switch (room.roomData[i].objectType) {
		case STATIC_OBJECT:
			createObject(levelObjects.levelStaticObjects, room.roomData[i], texture);
			break;
		case STATIC_PLATFORM:
			createObject(levelObjects.levelStaticPlatforms, room.roomData[i], texture);
			break;
		case STATIC_STAIRS:
			createObject(levelObjects.levelStaticStairs, room.roomData[i], texture);
			break;
		case ENEMY_MOVING:
			createObject(levelObjects.enemiesMoving, room.roomData[i], texture);
			break;
		case ENEMY_STATIC:
			createObject(levelObjects.enemiesStatic, room.roomData[i], texture);
			break;
		case STATIC_SPRITE:
			createObject(levelObjects.spritesStatic, room.roomData[i], texture);
			break;
		case PICK_UP:
			createObject(levelObjects.pickups, room.roomData[i], texture);
			break;
		default:
			//std::cout << "error in room data" << std::endl;
			break;
		}
	}	
	return room;
}

void GameUpdates::checkLevelChange(LevelObjects &levelObjects) {
	char playerState = levelObjects.player->externalCheckState();
	const char LEFT = 'l', RIGHT = 'r', UP = 'U', DOWN = 'D';
	switch (playerState) {
	case LEFT:
		changeLevel(nextRoomLeft, levelObjects);
		break;
	case RIGHT:
		changeLevel(nextRoomRight, levelObjects);
		break;
	case DOWN:
		changeLevel(nextRoomDown, levelObjects);
		break;
	case UP:
		changeLevel(nextRoomUp, levelObjects);
		break;
	default:
		break;
	}
}

void GameUpdates::clearRoomObjects(LevelObjects &levelObjects) {
	clearVector(levelObjects.enemiesMoving);
	clearVector(levelObjects.enemiesStatic);
	clearVector(levelObjects.levelStaticObjects);
	clearVector(levelObjects.levelStaticPlatforms);
	clearVector(levelObjects.levelStaticStairs);
	clearVector(levelObjects.pickups);
	clearVector(levelObjects.spritesStatic);
}

void GameUpdates::changeLevel(int nextRoom, LevelObjects &levelObjects) {
	clearRoomObjects(levelObjects);
	levelObjects.collision->clearCollisionData();

	levelObjects.room = createRoom(levelObjects.world.fileNames[nextRoom], levelObjects);
	nextRoomRight = levelObjects.room.roomId + 1;
	nextRoomLeft = levelObjects.room.roomId - 1;
	nextRoomUp = levelObjects.room.roomId - 10;
	nextRoomDown = levelObjects.room.roomId + 10;
	
	firstLoopComplete = false;
	inLevel = true;
	levelObjects.player->externalResetState();
}

void GameUpdates::updateGame(float dt, LevelObjects &levelObjects, Game &game) {
																  
	switch (game) {
	case Game::TITLE_SCREEN:		 
	 	levelObjects.titleScreen->update(dt);
		if (!levelObjects.titleScreen->getTitleScreenStatus()) {
			game = Game::GAME;
			break;
		}
		break;
	case Game::GAME:
		update(levelObjects.player, levelObjects.collision, dt);
		if (isFirstRun) {
			levelObjects.room = createRoom(levelObjects.world.fileNames[currentRoom], levelObjects);
			nextRoomRight = levelObjects.room.roomId + 1;
			nextRoomLeft = levelObjects.room.roomId - 1;
			nextRoomUp = levelObjects.room.roomId - 10;
			nextRoomDown = levelObjects.room.roomId + 10; 
			isFirstRun = false;
		}

		if (inLevel) {
			if (!firstLoopComplete) {
				update(levelObjects.levelStaticObjects, levelObjects.collision);
				update(levelObjects.levelStaticPlatforms, levelObjects.collision);				
				firstLoopComplete = true;
			}
			update(levelObjects.levelStaticStairs, levelObjects.collision, levelObjects.player);
			update(levelObjects.enemiesMoving, levelObjects.collision, levelObjects.player, dt);
			update(levelObjects.enemiesStatic, levelObjects.collision, levelObjects.player, dt);
			update(levelObjects.pickups, levelObjects.collision, levelObjects.player, dt);
			update(levelObjects.spritesStatic, dt);
		}
		checkLevelChange(levelObjects);
		break;
	case Game::DEAD:
		break;
	}
}
