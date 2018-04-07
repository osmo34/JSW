#include "MediaPlayer.h"   

MediaPlayer::MediaPlayer(int volume) : volume(volume) {}
MediaPlayer::~MediaPlayer(){}

void MediaPlayer::playMusic(std::string sound) {
	if (!music.openFromFile(sound)) { std::cout << sound << " failed to load" << std::endl; }
	music.setVolume(volume);
	music.play();
}

void MediaPlayer::stopMusic() {
	music.stop();
}																										 

void MediaPlayer::playSoundEffect(std::string soundEffect, float volume) {
	if (!buffer.loadFromFile(soundEffect)) { std::cout << soundEffect << " failed to load" << std::endl;}
	sound.setBuffer(buffer);
	sound.setVolume(volume);
	sound.play();
}
