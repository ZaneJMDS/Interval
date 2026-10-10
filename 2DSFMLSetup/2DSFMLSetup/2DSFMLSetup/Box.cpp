#include "Box.h"

Box::Box(sf::Vector2f _start_pos)
{
	start_pos = _start_pos;
	
	box_shape.setSize({ 64, 64 });
	box_shape.setTextureRect(sf::IntRect({ 112, 48 }, { 16, 16 }));
}

Box::~Box()
{
}
