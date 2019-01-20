#include "GameManager.h"

GameManager::GameManager(int screenWidth, int screenHeight) : m_screenWidth(screenWidth), m_screenHeight(screenHeight){}
GameManager::~GameManager(){}

void GameManager::initializeWorld()
{
	WorldFileMaster worldFile = readRoomFile();

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			std::string output = worldFile.worldFile[i][j].world;
			levelObjects.world.fileNames[i][j] = output;
		}
	}
}

void GameManager::initializeTextures()
{
	loadTextures = std::unique_ptr <LoadTextFile>(new LoadTextFile(TEXTURES_FILE_NAME));
	loadTextures->loadFile(textureList);
}

void GameManager::initializeTitleScreen()
{
	sf::Texture titleTexture;
	if (!titleTexture.loadFromFile("title.png")) { std::cout << "error with title screen image file"; }
	levelObjects.titleScreen = std::unique_ptr <TitleScreen>(new TitleScreen(m_screenWidth, m_screenHeight, titleTexture));
}

void GameManager::loadTexture(std::map<int, sf::Texture>& textures, std::string fileName, int id) {
	sf::Texture texture;
	if (!texture.loadFromFile(fileName)) { std::cout << "texture load failure - " << fileName; }
	textures[id] = texture;	
}

// read room file
WorldFileMaster GameManager::readRoomFile() {
	WorldFileMaster worldFile{};
	std::ifstream inFile;
	inFile.open(WORLD_FILE_NAME, std::ios::binary);

	if (inFile.is_open()) {
		inFile.read(reinterpret_cast<char*>(&worldFile), sizeof(worldFile));
		inFile.close();
	}
	else {
		std::cout << "World File Read Failed";
	}
	return worldFile;
}				 				 

// set up the game
void GameManager::initializeGame() {
	//set up title screen
	initializeTitleScreen();

	// load textures
	initializeTextures();
	
	for (int i = 0; i < textureList.size(); i++) {
		loadTexture(levelObjects.textures, textureList[i], i);
	}	

	// load world file
	initializeWorld();

	// create collision
	levelObjects.collision = std::shared_ptr <Collision>(new Collision());

	// play music
	gameSounds = std::unique_ptr<GameSounds>(new GameSounds(musicVolume, soundEffectVolume));
	gameSounds->playMusic("Moonlight_Sonata.ogg");

	// set up game updates
	gameUpdates = std::unique_ptr<GameUpdates>(new GameUpdates(m_screenWidth, m_screenHeight));

	// set up drawing
	gameDraw = std::unique_ptr<GameDraw>(new GameDraw());

	// create player
	levelObjects.player = std::shared_ptr <Player>(new Player(m_screenWidth, m_screenHeight, levelObjects.textures[0]));
}

void GameManager::update(float dt, bool isPaused) {
	if (!isPaused) {
		if (dt >= 55) { // delta time bug when loading new levels. TODO: low priority set specific to frame rate
			dt = 0;
		}
		gameUpdates->updateGame(dt, levelObjects, game);
		gameSounds->checkPlayerSoundEffects(levelObjects);
	}
}

// TODO: refactor into draw class
void GameManager::draw(sf::RenderWindow *window) {
	gameDraw->draw(window, levelObjects, game);	
}
