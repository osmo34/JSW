#include "GameManager.h"

GameManager::GameManager(int screenWidth, int screenHeight) : m_screenWidth(screenWidth), m_screenHeight(screenHeight){}
GameManager::~GameManager(){}

void GameManager::clearRoomObjects(LevelObjects & levelObjects)	{
	clearVector(levelObjects.enemiesMoving);
	clearVector(levelObjects.enemiesStatic);
	clearVector(levelObjects.levelStaticObjects);
	clearVector(levelObjects.levelStaticPlatforms);
	clearVector(levelObjects.levelStaticStairs);
	clearVector(levelObjects.pickups);
}

Room GameManager::createRoom(std::string fileName, LevelObjects & levelObjects, std::map<int, sf::Texture>& textures) {
	const char PLAYER = 'p', STATIC_OBJECT = 's', ENEMY = 'e', STATIC_PLATFORM = 't', STATIC_STAIRS = 'l', ENEMY_MOVING = 'm', ENEMY_STATIC = 'n', PICK_UP = 'u';
	Room room{};

	std::shared_ptr<LoadRoom> loadLevel(new LoadRoom);
	room = loadLevel->loadRoom(fileName);

	for (int i = 0; i < room.numObjects; i++) {
		sf::Texture texture;
		texture = textures.find(room.roomData[i].textureId)->second;

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
		case PICK_UP:
			createObject(levelObjects.pickups, room.roomData[i], texture);
			break;
		default:
			std::cout << "error in room data";
		}
	}
	return room;
}

void GameManager::loadTexture(std::map<int, sf::Texture>& textures, std::string fileName, int id) {
	sf::Texture texture;
	if (!texture.loadFromFile(fileName)) { std::cout << "texture load failure - " << fileName; }
	textures[id] = texture;
}

void GameManager::initializeGame() {
	// load textures
	loadTextures = std::shared_ptr <LoadTextFile>(new LoadTextFile(TEXTURES_FILE_NAME));																					  
	loadTextures->loadFile(textureList);
	
	for (int i = 0; i < textureList.size(); i++) {
		loadTexture(textures, textureList[i], i);
	}
	// load world
	loadWorld = std::shared_ptr <LoadTextFile>(new LoadTextFile(WORLD_FILE_NAME));
	loadWorld->loadFile(worldList);

	for (int i = 0; i < worldList.size(); i++) {
		world.fileNames[i] = worldList[i];
	}

	// create player
	player = std::shared_ptr <Player>(new Player(m_screenWidth, m_screenHeight, textures[0]));

	// create collision
	collision = std::shared_ptr <Collision>(new Collision());

	// play music
	mediaPlayer = std::unique_ptr<MediaPlayer>(new MediaPlayer(20));
	mediaPlayer->playMusic("Moonlight_Sonata.ogg");
}

void GameManager::checkLevelChange() {
	char playerState = player->externalCheckState();
	const char LEFT = 'l', RIGHT = 'r';
	switch (playerState) {
	case LEFT:
		changeLevel(nextRoomLeft);
		break;
	case RIGHT:
		changeLevel(nextRoomRight);
		break;
	default:
		break;
	}
}

// TODO: Will likely require an overiden method for top/bottom
void GameManager::changeLevel(int nextRoom) {
	clearRoomObjects(levelObjects);
	collision->clearCollisionData();
	room = createRoom(world.fileNames[nextRoom], levelObjects, textures);
	nextRoomRight = room.roomId + 1;
	nextRoomLeft = room.roomId - 1;
	firstLoopComplete = false;
	inLevel = true;
	player->externalResetState();
}

void GameManager::update(float dt) { 
	update(player, collision, dt);
	if (isFirstRun) {
		room = createRoom(world.fileNames[currentRoom], levelObjects, textures);
		nextRoomRight = room.roomId + 1;
		nextRoomLeft = room.roomId - 1;
		isFirstRun = false;
	}

	if (inLevel) {
		if (!firstLoopComplete) {
			update(levelObjects.levelStaticObjects, collision);
			update(levelObjects.levelStaticPlatforms, collision);
			firstLoopComplete = true;
		}
		update(levelObjects.levelStaticStairs, collision, player);
		update(levelObjects.enemiesMoving, collision, player, dt);
		update(levelObjects.enemiesStatic, collision, player, dt);
		update(levelObjects.pickups, collision, player, dt);
	}
	checkLevelChange();
}

void GameManager::draw(sf::RenderWindow *window) {
	draw(levelObjects.levelStaticObjects, window);
	draw(levelObjects.levelStaticPlatforms, window);
	draw(levelObjects.levelStaticStairs, window);
	draw(levelObjects.enemiesMoving, window);
	draw(levelObjects.enemiesStatic, window);
	draw(levelObjects.pickups, window);
	draw(player, window);
}