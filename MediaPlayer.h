#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>

class MediaPlayer
{
public:
	MediaPlayer(int volume);
	~MediaPlayer();
	void playMusic(std::string sound);
	void playSoundEffect(std::string soundEffect, float volume);
	void stopMusic();

private:
	int volume;
	sf::SoundBuffer buffer;
	sf::Sound sound;
	sf::Music music;
};

