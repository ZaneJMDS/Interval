#include <SFML/Audio.hpp>
#include <string>
#pragma once

class Audio
{
public:
	Audio(std::string _src);
	~Audio();

	void Play() { sound.play(); }
	void Stop() { sound.stop(); }

private:
	sf::SoundBuffer buffer;
	sf::Sound sound;
	std::string src;
};

