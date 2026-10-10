/***********************************************************************
Author      :	Zane Jackson
Mail        :   Zane.Jackson@mds.ac.nz
Description :	Class for tracking and reseting the location of boxes
File name   :   Box.h
**************************************************************************/

#pragma once
#include <SFML/Graphics.hpp>

class Box
{
public:
	sf::RectangleShape box_shape;

	Box(sf::Vector2f _start_pos);
	~Box();

	void ResetPosition() { box_shape.setPosition(start_pos); }

private:
	sf::Vector2f start_pos;
};

