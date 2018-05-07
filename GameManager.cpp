#include "GameManager.h"

GameManager::GameManager(int screenWidth, int screenHeight) : m_screenWidth(screenWidth), m_screenHeight(screenHeight){}
GameManager::~GameManager(){}

void GameManager::loadTexture(std::map<int, sf::Texture>& textures, std::string fileName, int id) {
	sf::Texture texture;
	if (!texture.loadFromFile(fileName)) { std::cout << "texture load failure - " << fileName; }
	textures[id] = texture;	
}

void GameManager::initializeGame() {
	//set up title screen
	sf::Texture titleTexture;
	if (!titleTexture.loadFromFile("title.png")) {	std::cout << "error with title screen image file"; }
	levelObjects.titleScreen = std::unique_ptr <TitleScreen>(new TitleScreen(m_screenWidth, m_screenHeight, titleTexture));

	// load textures
	loadTextures = std::unique_ptr <LoadTextFile>(new LoadTextFile(TEXTURES_FILE_NAME));																					  
	loadTextures->loadFile(textureList);
	
	for (int i = 0; i < textureList.size(); i++) {
		loadTexture(levelObjects.textures, textureList[i], i);
	}
	// load world
	loadWorld = std::unique_ptr <LoadTextFile>(new LoadTextFile(WORLD_FILE_NAME));
	loadWorld->loadFile(worldList);

	for (int i = 0; i < worldList.size(); i++) {
		levelObjects.world.fileNames[i] = worldList[i];		
	}

	// create player
	levelObjects.player = std::shared_ptr <Player>(new Player(m_screenWidth, m_screenHeight, levelObjects.textures[0]));

	// create collision
	levelObjects.collision = std::shared_ptr <Collision>(new Collision());

	// play music
	gameSounds = std::unique_ptr<GameSounds>(new GameSounds(musicVolume, soundEffectVolume));
	gameSounds->playMusic("Moonlight_Sonata.ogg");

	// set up game updates
	gameUpdates = std::unique_ptr<GameUpdates>(new GameUpdates(m_screenWidth, m_screenHeight));

	// set up drawing
	gameDraw = std::unique_ptr<GameDraw>(new GameDraw());
}

void GameManager::update(float dt) {
	gameUpdates->updateGame(dt, levelObjects, game);
	gameSounds->checkPlayerSoundEffects(levelObjects);
}

// TODO: refactor into draw class
void GameManager::draw(sf::RenderWindow *window) {
	gameDraw->draw(window, levelObjects, game);	
}