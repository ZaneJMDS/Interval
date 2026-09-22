#pragma once
#include <SFML/Graphics.hpp>

class Player
{
public:	
	// The player shape and texture should remain
	sf::RectangleShape player_shape;
	sf::Texture player_texture;

	Player();
	~Player();

	void Rotate() { player_shape.rotate(sf::degrees(180)); }
	void ResetPosition() { player_shape.setPosition({ 200.f, 450.f }); }
	void Gravity(float _playerY_vel) { player_shape.move({ 0.f, _playerY_vel }); }
	void Move(float _playerX_vel) { player_shape.move({ _playerX_vel, 0.f }); }
	sf::RectangleShape GetPlayerShape() { return player_shape; }

private:
	const float player_size = 50.f;
};

