#include "Audio.h"

Audio::Audio(std::string _src) : sound(buffer)
{
	src = _src;

	// Throw Error if mispelled the file path
	if (!buffer.loadFromFile(src))
	{
		throw "Error loading sound";
	}

	sound.setBuffer(buffer);
	sound.setVolume(50.f);
}

Audio::~Audio()
{
}
