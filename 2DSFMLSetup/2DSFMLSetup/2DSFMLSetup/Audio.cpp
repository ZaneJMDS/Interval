#include "Audio.h"

Audio::Audio(std::string _src) : src(_src)
{
	// Throw Error if mispelled the file path
	if (!sound.openFromFile(src))
	{
		throw "Error loading sound";
	}

	sound.setVolume(25.f);
	sound.setLooping(true);
}

Audio::~Audio()
{
}
