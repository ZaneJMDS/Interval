/***********************************************************************
Author      :	Zane Jackson
Mail        :   Zane.Jackson@mds.ac.nz
Description :	Class for managing the player object
File name   :   Player.h
**************************************************************************/

#pragma once
#include <SFML/Graphics.hpp>
#include "Audio.h"

class Player
{
public:	
	// The player shape and texture should remain
	sf::RectangleShape player_shape;
	sf::Texture player_texture;

	float Yvelocity = 0.f;
	float Xvelocity = 0.f;

	Player();
	~Player();

	void Rotate() { player_shape.rotate(sf::degrees(180)); }
	void ResetPosition();
	void Gravity() { player_shape.move({ 0.f, Yvelocity }); }
	void Jump();
	void Move() { player_shape.move({ Xvelocity, 0.f }); }
	void Animate();

	// Update player's position relative to gravity
	void UpdatePlayer(float _Yvelocity, float _dt);

private:
	const float player_size = 48.f;
	sf::Clock animation_clock;

	// For Sound
	sf::SoundBuffer jump_buffer;
	sf::Sound jump_sound;
};

