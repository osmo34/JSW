#include "GameManager.h"
// TODO: Split into smaller classes still - avoid bloat						   

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
	if (!titleTexture.loadFromFile("title.png")) {	std::cout << "error"; }
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
	mediaPlayer = std::unique_ptr<MediaPlayer>(new MediaPlayer(musicVolume));	
	mediaPlayer->playMusic("Moonlight_Sonata.ogg");

	// set up game updates
	gameUpdates = std::unique_ptr<GameUpdates>(new GameUpdates(m_screenWidth, m_screenHeight));
}

// TODO: Refactor into seperate class
void GameManager::checkSoundEffects() {
	char playerState = levelObjects.player->externalCheckState();
	const char JUMP = 'j', DEAD = 'd', PICK_UP = 'u';
	auto playSound = [&](std::string sound, bool &b, int volume) { mediaPlayer->playSoundEffect(sound, volume); b = true;  };

	switch (playerState) {
	case JUMP:
		if (!playedJumpSound) {
			playSound("126416__cabeeno-rossley__jump.wav", playedJumpSound, soundEffectVolume);
		}
		break;
	case DEAD:
		if (!playedDeadSound) {
			playSound("126416__cabeeno-rossley__jump.wav", playedDeadSound, soundEffectVolume);
		}
		break;
	case PICK_UP:
		if (!playedPickupSound) {
			playSound("126416__cabeeno-rossley__jump.wav", playedPickupSound, soundEffectVolume);
		}
		break;
	default:
		playedJumpSound = false;
		playedDeadSound = false;
		playedPickupSound = false;
		break;
	}
}

void GameManager::update(float dt) {
	gameUpdates->updateGame(dt, levelObjects, game);
	checkSoundEffects();	
}

// TODO: refactor into draw class
void GameManager::draw(sf::RenderWindow *window) {
	switch (game) {

	case Game::TITLE_SCREEN:
		levelObjects.titleScreen->draw(window);
		break;
	case Game::GAME:
		draw(levelObjects.levelStaticObjects, window);
		draw(levelObjects.levelStaticPlatforms, window);
		draw(levelObjects.levelStaticStairs, window);
		draw(levelObjects.enemiesMoving, window);
		draw(levelObjects.enemiesStatic, window);
		draw(levelObjects.pickups, window);
		draw(levelObjects.player, window);
		break;
	case Game::DEAD:
		break;
	}
}