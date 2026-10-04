#include "Box.h"

Box::Box(sf::RectangleShape* NewBox)
{
	start_pos = NewBox->getPosition();
}

Box::~Box()
{
}
