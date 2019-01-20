#include "GameManager.h"

GameUpdates::GameUpdates(int screenWidth, int screenHeight) : screenWidth(screenWidth), screenHeight(screenHeight) {
	isFirstRun = true;
	inLevel = true;
	firstLoopComplete = false;
}

GameUpdates::~GameUpdates() {}

Room GameUpdates::createRoom(std::string fileName, LevelObjects & levelObjects) {
	const char PLAYER = 'p', STATIC_OBJECT = 's', ENEMY = 'e', STATIC_PLATFORM = 't', 
		STATIC_STAIRS = 'l', STATIC_SPRITE = '#', STATIC_BACKGROUND = '~', ENEMY_MOVING = 'm', 
		ENEMY_STATIC = 'n', PICK_UP = 'u', ROPE = 'r';
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
			if (room.roomData[i].isTravelator) {
				createObject(levelObjects.travelators, room.roomData[i], texture);
				break;
			}			
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
		case STATIC_BACKGROUND:
			createObject(levelObjects.spritesBackgroundLayer, room.roomData[i], texture);
			break;
		case PICK_UP:
			createObject(levelObjects.pickups, room.roomData[i], texture);
			break;
		case ROPE:
			createObject(levelObjects.ropes, room.roomData[i], texture);
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
		changeLevel(currentRoomVertical, nextRoomLeft, levelObjects);
		break;
	case RIGHT:
		changeLevel(currentRoomVertical, nextRoomRight, levelObjects);
		break;
	case DOWN:
		changeLevel(nextRoomDown, currentRoomHorizontal, levelObjects);
		break;
	case UP:
		changeLevel(nextRoomUp, currentRoomHorizontal, levelObjects);
		break;
	default:
		break;
	}
}

void GameUpdates::clearRoomObjects(LevelObjects &levelObjects) {
	clearVector(levelObjects.enemiesMoving);
	clearVector(levelObjects.enemiesStatic);
	clearVector(levelObjects.levelStaticObjects);
	clearVector(levelObjects.spritesBackgroundLayer);
	clearVector(levelObjects.levelStaticPlatforms);
	clearVector(levelObjects.levelStaticStairs);
	clearVector(levelObjects.pickups);
	clearVector(levelObjects.spritesStatic);
	clearVector(levelObjects.ropes);
	clearVector(levelObjects.travelators);
}

void GameUpdates::changeLevel(int nextRoomVertical, int nextRoomHorizontal, LevelObjects &levelObjects) {
	clearRoomObjects(levelObjects);
	levelObjects.collision->clearCollisionData();

	levelObjects.room = createRoom(levelObjects.world.fileNames[nextRoomVertical][nextRoomHorizontal], levelObjects);
	nextRoomRight = levelObjects.room.roomId + 1;
	nextRoomLeft = levelObjects.room.roomId - 1;
	nextRoomUp = levelObjects.room.roomLevelVertical - 1;
	nextRoomDown = levelObjects.room.roomLevelVertical + 1;
	
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
		
		if (isFirstRun) {
			levelObjects.room = createRoom(levelObjects.world.fileNames[currentRoomVertical][currentRoomHorizontal], levelObjects);
			nextRoomRight = levelObjects.room.roomId + 1;
			nextRoomLeft = levelObjects.room.roomId - 1;
			nextRoomUp = levelObjects.room.roomLevelVertical - 1;
			nextRoomDown = levelObjects.room.roomLevelVertical + 1; 
			isFirstRun = false;
		}

		if (inLevel) {	
			update(levelObjects.player, levelObjects.collision, dt);
			if (!firstLoopComplete) { 
				update(levelObjects.levelStaticPlatforms, levelObjects.collision);
				update(levelObjects.levelStaticObjects, levelObjects.collision);
				update(levelObjects.travelators, levelObjects.collision, NULL);
				firstLoopComplete = true;
			}  
			update(levelObjects.levelStaticStairs, levelObjects.collision, levelObjects.player);
			update(levelObjects.enemiesMoving, levelObjects.collision, levelObjects.player, dt);
			update(levelObjects.enemiesStatic, levelObjects.collision, levelObjects.player, dt);
			update(levelObjects.pickups, levelObjects.collision, levelObjects.player, dt);
			update(levelObjects.spritesStatic, dt);
			
			// TODO: temp before template
			for (auto i : levelObjects.ropes) {
				i->update(dt);
			}			
		}
		checkLevelChange(levelObjects);
		break;
	case Game::DEAD:
		break;
	}
}
