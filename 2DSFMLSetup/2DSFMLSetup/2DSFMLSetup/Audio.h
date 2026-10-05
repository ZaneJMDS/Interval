#include <SFML/Audio.hpp>
#include <string>
#pragma once

class Audio
{
public:
	Audio(std::string _src);
	~Audio();

	sf::Sound GetSound() { return sound; }
	void Play() { sound.play(); }

private:
	sf::SoundBuffer buffer;
	sf::Sound sound;
	std::string src;
};

