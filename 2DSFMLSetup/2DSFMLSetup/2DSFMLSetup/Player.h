#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Player
{
public:	
	// The player shape and texture should remain
	sf::RectangleShape player_shape;
	sf::Texture player_texture;

	Player();
	~Player();

	void Rotate() { player_shape.rotate(sf::degrees(180)); }
	void ResetPosition();
	void Gravity(float _playerY_vel) { player_shape.move({ 0.f, _playerY_vel }); }
	void Jump(float _playerY_vel);
	void Move(float _playerX_vel) { player_shape.move({ _playerX_vel, 0.f }); }

	// Update player's position
	float UpdatePlayer(float _playerYvel, float _Yvelocity, float _dt);

private:
	const float player_size = 50.f;
};

