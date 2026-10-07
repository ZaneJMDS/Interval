/***********************************************************************
Author      :	Zane Jackson
Mail        :   Zane.Jackson@mds.ac.nz
Description :	Class for slime enemy
File name   :   Enemy.h
**************************************************************************/

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Enemy
{
public:
	sf::RectangleShape enemy_shape;

	Enemy(sf::Vector2f _start_pos);
	~Enemy();

	void Animate();
	void Reset();
	void Rotate();
	void Move();

private:
	const float enemy_size = 48.f;
	sf::Vector2f start_pos;
	sf::Clock animation_clock;
};

