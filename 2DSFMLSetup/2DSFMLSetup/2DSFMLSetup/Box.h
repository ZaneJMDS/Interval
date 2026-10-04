#pragma once
#include <SFML/Graphics.hpp>

class Box
{
public:

	// Class for tracking and reseting the location of boxes
	Box(sf::RectangleShape* NewBox);
	~Box();

	void ResetPosition(sf::RectangleShape* NewBox) { NewBox->setPosition(start_pos); }

private:
	sf::Vector2f start_pos;
};

