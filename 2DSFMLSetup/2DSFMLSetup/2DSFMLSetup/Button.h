/***********************************************************************
Author      :	Zane Sebastian Jackson
Mail        :   Zane.Jackson@mds.ac.nz
Description :	Class for Menu Buttons
File name   :   Button.h
**************************************************************************/

#pragma once
#include <SFML/Graphics.hpp>

class Button
{
public:
	sf::RectangleShape m_ButtonShape;

	Button(sf::Vector2f _position, sf::Color _color, sf::Text _text);
	~Button();
private:
};


