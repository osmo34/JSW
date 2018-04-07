#include "MusicPlayer.h"   

MusicPlayer::MusicPlayer(){}
MusicPlayer::~MusicPlayer(){}

void MusicPlayer::playMusic(std::string sound) {
	if (!music.openFromFile(sound)) { std::cout << sound << " failed to load" << std::endl; }
	music.play();
}

void MusicPlayer::stopMusic() {
	music.stop();
}
