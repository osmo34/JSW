#include "GameManager.h"

GameSounds::GameSounds(int musicVolume, int soundEffectVolume) : musicVolume(musicVolume), soundEffectVolume(soundEffectVolume) {
	initializeMediaPlayer();
}

GameSounds::~GameSounds(){}

void GameSounds::initializeMediaPlayer() {
	mediaPlayer = std::unique_ptr<MediaPlayer>(new MediaPlayer(musicVolume));
}

void GameSounds::playMusic(std::string song) {
	mediaPlayer->playMusic(song);
}

void GameSounds::checkPlayerSoundEffects(LevelObjects &levelObjects) {
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

