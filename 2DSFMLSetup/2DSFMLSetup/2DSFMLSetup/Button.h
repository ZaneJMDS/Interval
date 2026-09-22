#pragma once
#include <SFML/Graphics.hpp>

class Button
{
public:
	sf::Font font();
	sf::RectangleShape m_ButtonShape;

	Button(sf::Vector2f _position, sf::Color _color);
	~Button();
};


