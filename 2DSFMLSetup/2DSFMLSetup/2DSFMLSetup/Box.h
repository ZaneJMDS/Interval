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
	Box(sf::RectangleShape* NewBox);
	~Box();

	void ResetPosition(sf::RectangleShape* NewBox) { NewBox->setPosition(start_pos); }

private:
	sf::Vector2f start_pos;
};

