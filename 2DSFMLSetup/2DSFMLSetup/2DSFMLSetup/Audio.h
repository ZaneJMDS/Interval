/***********************************************************************
Author      :	Zane Jackson
Mail        :   Zane.Jackson@mds.ac.nz
Description :	Class for adjusting volume of audio
File name   :   Audio.h
**************************************************************************/

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
	void IncreaseVolume() { if (sound.getVolume() < 49.f) sound.setVolume(sound.getVolume() + 5.f); }
	void DecreaseVolume() { if (sound.getVolume() > 1.f) sound.setVolume(sound.getVolume() - 5.f); }


private:
	sf::SoundBuffer buffer;
	sf::Sound sound;
	std::string src;
};

