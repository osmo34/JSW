#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>

class MusicPlayer
{
public:
	MusicPlayer();
	~MusicPlayer();
	void playMusic(std::string sound);
	void stopMusic();

private:
	sf::Music music;
};

